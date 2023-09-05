#include "Player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

Player::Player() {
    this->init_player();
}

void Player::init_player() {
    if(!this->player_texture_.loadFromFile("resources/Player.PNG")) return;
    this->player_sprite_.setTexture(this->player_texture_);
    this->player_sprite_.setPosition(this->x_position_,this->y_position_);
    this->player_sprite_.setScale(this->scale_player_,this->scale_player_);
    this->prev_direction_ = Direction::RIGHT;
    this->flip_player();
}

void Player::flip_player() {
    float prev_direction_flag = static_cast<float>(this->prev_direction_);
    this->player_sprite_.setScale(-1.f*this->scale_player_*prev_direction_flag, this->scale_player_);
    this->player_sprite_.setOrigin(((prev_direction_flag+1)/2.f)*this->player_sprite_.getGlobalBounds().width/this->scale_player_, 0.f);
    this->prev_direction_ = static_cast<Direction>(-1*prev_direction_flag);
}

bool Player::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

void Player::move_player(Direction direction) {
    float bottom = this->player_sprite_.getGlobalBounds().top + this->player_sprite_.getGlobalBounds().height + 10.f;
    switch (direction) {
    case Direction::LEFT:
            this->player_sprite_.move(-this->player_speed_, 0.f);
            if (this->direction_changed(direction)) this->flip_player();
            break;
    case Direction::RIGHT:
            this->player_sprite_.move(this->player_speed_, 0.f);
            if (this->direction_changed(direction)) this->flip_player();
            break;
    case Direction::UP:
            if (this->player_sprite_.getGlobalBounds().top > this->y_min_pos_) this->player_sprite_.move(0.f, -this->player_speed_);
            break;
    case Direction::DOWN:
            if(bottom < this->y_max_pos_) this->player_sprite_.move(0.f, this->player_speed_);
            break;
    default: return;
    }
}

void Player::render(sf::RenderTarget &target) {
    target.draw(player_sprite_);
}

float Player::get_x_default_right() const { return this->x_default_right; }
float Player::get_x_default_left() const { return this->x_default_left; }
