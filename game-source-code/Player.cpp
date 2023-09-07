#include "Player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "Bullet.h"

Player::Player(sf::Texture& texture) {
    this->init_player(texture);
}

void Player::init_player(sf::Texture& texture) {
    this->player_sprite_.setTexture(texture);
    this->player_sprite_.setTextureRect(sf::IntRect(1054.f, 0.f, 985.f, 414.f));
    this->player_sprite_.setScale(this->scale_player_,this->scale_player_);
    this->player_sprite_.setPosition(this->x_default_right_-this->player_sprite_.getGlobalBounds().width,this->default_y_);
    this->prev_direction_ = Direction::LEFT;
    this->prev_vertical_direction_ = Direction::LEFT; // Prevent initial move
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

void Player::move_player_vertical(Direction direction, bool pressed) {
    float bottom = this->player_sprite_.getGlobalBounds().top + this->player_sprite_.getGlobalBounds().height + 10.f;
    if (pressed) this->vertical_speed_ = this->vertical_base_speed;
    else if (this->vertical_speed_ > 0) this->vertical_speed_ -= this->vertical_acceleration_;
    switch (direction) {
        case Direction::UP:
                if (this->player_sprite_.getGlobalBounds().top > this->y_min_pos_) {
                    this->player_sprite_.move(0.f, -this->vertical_speed_);
                }
                this->prev_vertical_direction_ = Direction::UP;
                break;
        case Direction::DOWN:
                if(bottom < this->y_max_pos_) {
                    this->player_sprite_.move(0.f, this->vertical_speed_);
                }
                this->prev_vertical_direction_ = Direction::DOWN;
                break;
        default: return;
    }
}

void Player::move_player_horizontal(Direction direction) {
    this->player_speed_ += this->acceleration_;
    this->player_sprite_.move(-static_cast<float>(direction)*this->player_speed_, 0.f);
    if (this->direction_changed(direction)) this->flip_player();
    this->prev_direction_ = direction;
}

void Player::edge_player_movement(Direction direction) {
    this->player_edge_speed = 0.5f;
    this->player_sprite_.move(static_cast<float>(direction)*player_edge_speed, 0.f);
    if (this->direction_changed(direction)) this->flip_player();
    this->prev_direction_ = direction;

}

void Player::edge_decelerate() {
    if(player_edge_speed>=0) this->player_edge_speed -= this->edge_acceleration_;
    this->player_sprite_.move(static_cast<float>(this->prev_direction_)*player_edge_speed, 0.f);
}

void Player::correct_edge_positions() {
    if (this->player_sprite_.getGlobalBounds().left < 700.f) {
        this->player_sprite_.setPosition(this->x_default_left_, this->player_sprite_.getGlobalBounds().top);
        return;
    }
    this->player_sprite_.setPosition(this->x_default_right_, this->player_sprite_.getGlobalBounds().top);
}

void Player::magnetise_player() {
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

void Player::render_bullets(sf::RenderTarget &target, float background_movement) {
    for (auto i = 0; i < this->bullets_.size(); i++) {
         if(this->bullets_[i]->get_location().x > 0.f && this->bullets_[i]->get_location().x < 1400.f){
            this->bullets_[i]->move_bullet(background_movement);
            this->bullets_[i]->draw_bullet(target);
        } else erase_bullet(i);
    }
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

std::vector<std::shared_ptr<Bullet>> Player::get_bullets() const { return this->bullets_; }

void Player::shoot_bullet(sf::Texture& texture) {
    if (this->current_cool_down < this->max_cool_down) return;
    else this->current_cool_down = 0;
    this->player_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 985.f, 414.f));
    auto x_position = 0.f;
    auto y_position = this->player_sprite_.getGlobalBounds().top + this->player_sprite_.getGlobalBounds().height - 11.f;
    if (this->prev_direction_ == Direction::LEFT) {
        x_position = this->player_sprite_.getGlobalBounds().left + 2*this->player_sprite_.getGlobalBounds().width;
    }
    else x_position = this->player_sprite_.getGlobalBounds().left;
    this->bullets_.push_back(std::make_shared<Bullet>(x_position, y_position, this->prev_direction_, texture));
}

void Player::increment_cool_down() {
    if (this->current_cool_down > 100) this->player_sprite_.setTextureRect(sf::IntRect(1054.f, 0.f, 985.f, 414.f));
    this->current_cool_down++; 
}

void Player::erase_bullet(int position) {
    this->bullets_.erase(this->bullets_.begin()+position);
}

Direction Player::get_prev_vertical_direction() { return this->prev_vertical_direction_; }
