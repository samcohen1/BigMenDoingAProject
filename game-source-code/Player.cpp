/** \file Player.cpp
    \brief Contains the implementation of the player
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "Bullet.h"



//Constructor that initialises the player
Player::Player(sf::Texture& texture) {
    init_player(texture);
}

//Set's texture, sprite and position
void Player::init_player(sf::Texture& texture) {
    player_sprite_.setTexture(texture);
    player_sprite_.setTextureRect(sf::IntRect(1054.f, 0.f, 985.f, 414.f));
    player_sprite_.setScale(scale_player_, scale_player_);
    player_sprite_.setPosition(x_default_right_ - player_sprite_.getGlobalBounds().width, default_y_);
    prev_direction_ = Direction::LEFT;
    prev_vertical_direction_ = Direction::LEFT;
    flip_player();
}


void Player::flip_player() {

    //Turns the player to face the other direction
    player_speed_ = flip_base_speed_;
    float prev_direction_flag = static_cast<float>(prev_direction_);
    player_sprite_.setScale(-1.f * scale_player_ * prev_direction_flag, scale_player_);
    player_sprite_.setOrigin(((prev_direction_flag + 1) / 2.f) * player_sprite_.getGlobalBounds().width / scale_player_, 0.f);
    prev_direction_ = static_cast<Direction>(-1 * prev_direction_flag);
}


bool Player::direction_changed(Direction direction) { return prev_direction_ != direction; }


void Player::move_player_vertical(Direction direction, bool pressed) {

    //Moves the player up or down and determines the speed at which it must move
    float bottom = player_sprite_.getGlobalBounds().top + player_sprite_.getGlobalBounds().height + 10.f;
    if (pressed)
        vertical_speed_ = vertical_base_speed;
    else if (vertical_speed_ > 0)
        vertical_speed_ -= vertical_acceleration_;

    switch (direction) {
        case Direction::UP:
            if (player_sprite_.getGlobalBounds().top > y_min_pos_) {
                player_sprite_.move(0.f, -vertical_speed_);
            }
            prev_vertical_direction_ = Direction::UP;
            break;
        case Direction::DOWN:
            if (bottom < y_max_pos_) {
                player_sprite_.move(0.f, vertical_speed_);
            }
            prev_vertical_direction_ = Direction::DOWN;
            break;
        default:
            return;
    }
}


void Player::move_player_horizontal(Direction direction) {

    //Moves player horizontally based on direction
    player_speed_ += acceleration_;
    player_sprite_.move(-static_cast<float>(direction) * player_speed_, 0.f);
    if (direction_changed(direction))
        flip_player();
    prev_direction_ = direction;
}


void Player::correct_edge_positions() {
    if (player_sprite_.getGlobalBounds().left < 700.f) {
        player_sprite_.setPosition(x_default_left_ - player_sprite_.getGlobalBounds().width, player_sprite_.getGlobalBounds().top);
        return;
    }
    player_sprite_.setPosition(x_default_right_, player_sprite_.getGlobalBounds().top);
}


void Player::magnetise_player() {

    //Ensures that the player is sent to the edges of the screen and not in the middle
    float acceleration_constant = 0.1f;

    //Checks if the player is in the wrong position and corrects it
    if ((player_sprite_.getGlobalBounds().left <= x_default_left_ && prev_direction_ == Direction::RIGHT)) {
        float delta_x = -((player_sprite_.getGlobalBounds().left + player_sprite_.getGlobalBounds().width) - x_default_right_);
        float acceleration = acceleration_constant / delta_x;
        player_speed_ += acceleration;
        player_sprite_.move(-static_cast<float>(prev_direction_) * player_speed_, 0.f);
        return;
    } else if (player_sprite_.getGlobalBounds().left + player_sprite_.getGlobalBounds().width >= x_default_right_ && prev_direction_ == Direction::LEFT) {
        float delta_x = player_sprite_.getGlobalBounds().left - x_default_left_;
        float acceleration = acceleration_constant / delta_x;
        player_speed_ += acceleration;
        player_sprite_.move(-static_cast<float>(prev_direction_) * player_speed_, 0.f);
        return;
    }

    if (player_sprite_.getGlobalBounds().left <= x_default_left_ || player_sprite_.getGlobalBounds().left + player_sprite_.getGlobalBounds().width >= x_default_right_) {
        return;
    }

    float delta_x = prev_direction_ == Direction::LEFT ? (player_sprite_.getGlobalBounds().left - x_default_left_) : -((player_sprite_.getGlobalBounds().left + player_sprite_.getGlobalBounds().width) - x_default_right_);
    float acceleration = acceleration_constant / delta_x;
    player_speed_ += acceleration;
    player_sprite_.move(-static_cast<float>(prev_direction_) * player_speed_, 0.f);
}


void Player::render_bullets(sf::RenderTarget &target, float background_movement) {

    //Draws all of the player's bullets
    for (auto i = 0; i < bullets_.size(); i++) {
        if (bullets_[i]->get_location().x > 0.f && bullets_[i]->get_location().x < 1400.f) {
            bullets_[i]->move(background_movement);
            bullets_[i]->draw(target);
        } else
            erase_bullet(i);
    }
}


void Player::render(sf::RenderTarget &target, float background_movement) {
    target.draw(player_sprite_);
    render_bullets(target, background_movement);
}


float Player::get_x_default_right() const { return x_default_right_; }


float Player::get_x_default_left() const { return x_default_left_; }


float Player::get_player_speed() const { return player_speed_ * static_cast<float>(prev_direction_); }


Position Player::get_position() {

    auto global_position = Position();
    global_position.x_left = player_sprite_.getGlobalBounds().left;
    global_position.x_right = player_sprite_.getGlobalBounds().left + player_sprite_.getGlobalBounds().width;
    global_position.y = player_sprite_.getGlobalBounds().top;
    return global_position;
}


std::vector<std::shared_ptr<Bullet>> Player::get_bullets() const { return bullets_; }


void Player::shoot_bullet(sf::Texture& texture) {

    //Checks if the player is able to shoot a bullet and returns if it is not
    if (current_cool_down < max_cool_down)
        return;
    else
        current_cool_down = 0;

    //Changes the animation of the player to show it shooting
    player_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 985.f, 414.f));
    auto x_position = 0.f;
    auto y_position = player_sprite_.getGlobalBounds().top + player_sprite_.getGlobalBounds().height - 11.f;


    if (prev_direction_ == Direction::LEFT) {
        x_position = player_sprite_.getGlobalBounds().left + 1 * player_sprite_.getGlobalBounds().width;
    } else
        x_position = player_sprite_.getGlobalBounds().left;

    //Adds the bullet to the vector of bullets
    bullets_.push_back(std::make_shared<Bullet>(x_position, y_position, prev_direction_, texture, world_bounds_));
}


void Player::increment_cool_down() {
    if (current_cool_down > 100)
        player_sprite_.setTextureRect(sf::IntRect(1054.f, 0.f, 985.f, 414.f));
    current_cool_down++;
}


void Player::erase_bullet(int position) {
    bullets_.erase(bullets_.begin() + position);
}


void Player::erase_bullet(std::shared_ptr<Bullet> bullet) {
    auto it = std::find(bullets_.begin(), bullets_.end(), bullet);
    if(it != bullets_.end()) {
        bullets_.erase(it);
    } else std::cout << "bullet non existant cannot erase\n";
}


Direction Player::get_prev_vertical_direction() { return prev_vertical_direction_; }


void Player::communicate_position(float background_location) {
    auto new_world_bounds = sf::FloatRect(player_sprite_.getGlobalBounds().left - background_location, player_sprite_.getGlobalBounds().top, player_sprite_.getGlobalBounds().width, player_sprite_.getGlobalBounds().height);
    world_bounds_ = new_world_bounds;
}


sf::FloatRect Player::get_world_bounds() {
    return world_bounds_;
}
