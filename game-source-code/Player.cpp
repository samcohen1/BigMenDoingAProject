#include "Player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>

Player::Player() {
    this->init_player();
}

void Player::init_player() {
    if(!this->player_texture_.loadFromFile("resources/Player.PNG")) return;
    this->player_sprite_.setTexture(this->player_texture_);
    this->player_sprite_.setScale(this->scale_player_,this->scale_player_);
    this->player_sprite_.setPosition(this->x_default_right_-this->player_sprite_.getGlobalBounds().width,this->default_y_);
    this->prev_direction_ = Direction::LEFT;
    this->flip_player();
}

void Player::flip_player() {
    this->player_speed_ = this->flip_base_speed_;
    float prev_direction_flag = static_cast<float>(this->prev_direction_);
    this->player_sprite_.setScale(-1.f*this->scale_player_*prev_direction_flag, this->scale_player_);
    this->player_sprite_.setOrigin(((prev_direction_flag+1)/2.f)*this->player_sprite_.getGlobalBounds().width/this->scale_player_, 0.f);
    this->prev_direction_ = static_cast<Direction>(-1*prev_direction_flag);
}

bool Player::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

void Player::move_player_vertical(Direction direction) {
    float bottom = this->player_sprite_.getGlobalBounds().top + this->player_sprite_.getGlobalBounds().height + 10.f;
    switch (direction) {
        case Direction::UP:
                if (this->player_sprite_.getGlobalBounds().top > this->y_min_pos_) this->player_sprite_.move(0.f, -this->vertical_speed_);
                break;
        case Direction::DOWN:
                if(bottom < this->y_max_pos_) this->player_sprite_.move(0.f, this->vertical_speed_);
                break;
        default: return;
    }
}

void Player::move_player_horizontal(Direction direction) {
    this->player_speed_ += acceleration_;
    this->player_sprite_.move(-static_cast<float>(direction)*this->player_speed_, 0.f);
    if (this->direction_changed(direction)) this->flip_player();
    this->prev_direction_ = direction;
}

void Player::edge_player_movement(Direction direction) {
    float player_edge_speed = 0.5f;
    this->player_sprite_.move(static_cast<float>(direction)*player_edge_speed, 0.f);
    if (this->direction_changed(direction)) this->flip_player();
    this->prev_direction_ = direction;
}

void Player::correct_edge_positions() {
    if (this->player_sprite_.getGlobalBounds().left < 700.f) {
        this->player_sprite_.setPosition(this->x_default_left_, this->player_sprite_.getGlobalBounds().top);
        return;
    }
    this->player_sprite_.setPosition(this->x_default_right_, this->player_sprite_.getGlobalBounds().top);
}

void Player::magnatise_player() {
    float acceleration_constant = 0.1f;
    if ((this->player_sprite_.getGlobalBounds().left <= this->x_default_left_ && this->prev_direction_ == Direction::RIGHT)) {
        float delta_x = -((this->player_sprite_.getGlobalBounds().left+this->player_sprite_.getGlobalBounds().width)-this->x_default_right_);
        float acceleration = acceleration_constant/delta_x;
        this->player_speed_ += acceleration;
        this->player_sprite_.move(-static_cast<float>(this->prev_direction_)*this->player_speed_, 0.f);
        return;
    } else if (this->player_sprite_.getGlobalBounds().left+this->player_sprite_.getGlobalBounds().width >= this->x_default_right_ && this->prev_direction_ == Direction::LEFT) {
        float delta_x = this->player_sprite_.getGlobalBounds().left-this->x_default_left_;
        float acceleration = acceleration_constant/delta_x;
        this->player_speed_ += acceleration;
        this->player_sprite_.move(-static_cast<float>(this->prev_direction_)*this->player_speed_, 0.f);
        return;
    }
    if(this->player_sprite_.getGlobalBounds().left <= this->x_default_left_ || this->player_sprite_.getGlobalBounds().left+this->player_sprite_.getGlobalBounds().width >= this->x_default_right_) {
        return;
    }
    float delta_x = this->prev_direction_ == Direction::LEFT ? (this->player_sprite_.getGlobalBounds().left-this->x_default_left_) : -((this->player_sprite_.getGlobalBounds().left+this->player_sprite_.getGlobalBounds().width)-this->x_default_right_);
    float acceleration = acceleration_constant/delta_x;
    this->player_speed_ += acceleration;
    this->player_sprite_.move(-static_cast<float>(this->prev_direction_)*this->player_speed_, 0.f);
}

void Player::render(sf::RenderTarget &target) {
    target.draw(player_sprite_);
}

float Player::get_x_default_right() const { return this->x_default_right_; }
float Player::get_x_default_left() const { return this->x_default_left_; }
float Player::get_player_speed() const { return this->player_speed_*static_cast<float>(this->prev_direction_); }
Position Player::get_position() { 
    auto global_position = Position();
    global_position.x_left = this->player_sprite_.getGlobalBounds().left;
    global_position.x_right = this->player_sprite_.getGlobalBounds().left + this->player_sprite_.getGlobalBounds().width;
    global_position.y = this->player_sprite_.getGlobalBounds().top;
    return global_position;
}
