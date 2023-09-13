/** \file Professor.cpp
    \brief Contains the implementation of the professor
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <random>

#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Professor.h"
#include "Bullet.h"
#include "Professor.h"
#include "Professor_Assignment.h"
#include "Throwable.h"

//Constructor for the professor, increments the number of professors static variable and sets up the professor
Professor::Professor(sf::Texture& texture) {
    num_professors_++;
    this->init_professor(texture);
    this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 488.f, 461.f, 380.f));
    this->professor_sprite_.setScale(this->scale_professor_, this->scale_professor_);
    this->professor_sprite_.setPosition(this->initial_x_position, this->initial_y_position);
}


Professor::~Professor() {
    //Decrement static number of professors
    num_professors_--;
}


int Professor::get_num_professors() { return num_professors_; }


void Professor::init_professor(sf::Texture& texture) {
    this->professor_sprite_.setTexture(texture);

    //Generate and set the professors movement function which is a sine wave with a differing period, amplitude and gradient-shift
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> amplitude_distributor(-400, 400);
    std::uniform_int_distribution<> period_distributor(10, 20);
    std::uniform_int_distribution<> gradient_distributor(-10, 10);
    std::uniform_int_distribution<> initial_x_distributor(-2777, 5650);
    std::uniform_int_distribution<> initial_y_distributor(120, 750);

    this->movement_amplitude = amplitude_distributor(generator);
    this->movement_period = float(period_distributor(generator)) / 1000;
    this->movement_gradient = float(gradient_distributor(generator)) / 200;

    // Initialize world position to the starting position.
    this->world_position.x = initial_x_distributor(generator);
    this->world_position.y = initial_y_distributor(generator);
    this->initial_x_position = this->world_position.x;
    this->initial_y_position = this->world_position.y;
    this->prev_y_position = this->world_position.y;

    int choose_direction = amplitude_distributor(generator);
    if (choose_direction > 0) {
        this->horizontal_speed_ *= -1;
        this->flip_professor();
    } else
        this->flip_professor();
}


float Professor::movement_function() {
    //Calculate the professor's y-value using the number of frames since he was spawned
    float x = this->frames_since_spawn * this->vertical_speed_;
    return this->movement_amplitude * std::sin(this->movement_period * x) + this->movement_gradient * x;
}

void Professor::flip_professor() {
    //Turn the professor around when applicable and make him move in the other direction
    this->horizontal_speed_ *= -1.f;
    int prev_direction_flag = this->horizontal_speed_ >= 0 ? 1 : -1;
    this->professor_sprite_.setScale(-prev_direction_flag * this->scale_professor_, this->scale_professor_);
    this->professor_sprite_.setOrigin(((prev_direction_flag + 1) / 2.f) * this->professor_sprite_.getGlobalBounds().width / this->scale_professor_, 0.f);
    this->initial_x_position = this->world_position.x;
    this->frames_since_spawn = 0;
}


void Professor::face_player(sf::Vector2f player_position) {

    //Turn the professor around to face the player
    if (this->professor_sprite_.getPosition().x < player_position.x) {
        this->professor_sprite_.setScale(-this->scale_professor_, this->scale_professor_);
        this->professor_sprite_.setOrigin(this->professor_sprite_.getGlobalBounds().width / this->scale_professor_, 0.f);
    } else {
        this->professor_sprite_.setScale(this->scale_professor_, this->scale_professor_);
        this->professor_sprite_.setOrigin(0.f, 0.f);
    }
}


bool Professor::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

void Professor::move_vertical () {

    //Check the total y-position and the difference from the previous y-position to know how much the professor should move
    this->frames_since_spawn++;
    int y = static_cast<int>(this->initial_y_position + this->movement_function());
    float move_y = y-this->prev_y_position;

    //Ensure the professor stays within bounds
    auto temp_pos = this->world_position.y + move_y;
    if (temp_pos <= this->y_min_pos_)  {
        if(move_y < 0) this->y_direction = -1;
        else this->y_direction = 1;
    }
    else if (temp_pos + this->professor_sprite_.getGlobalBounds().height >= this->y_max_pos_) {
        if(move_y > 0) this->y_direction = -1;
        else this->y_direction = 1;
    }
    this->world_position.y += this->y_direction*move_y;
    this->professor_sprite_.move(0.f, this->y_direction*move_y);
    this->prev_y_position = y;
}

void Professor::move_horizontal (float background_movement, float background_location) {

    //Calculate how much the professor should move
    float move_x = this->horizontal_speed_ + background_movement;
    auto temp_pos = this->world_position.x + move_x;

    //Ensure the professor remains in bounds
    if (temp_pos <= -2800+this->professor_sprite_.getGlobalBounds().width) move_x += 8500.f-1400.f;
    else if (temp_pos >= 5700-this->professor_sprite_.getGlobalBounds().width) move_x -= 8500.f-1400.f;
    this->world_position.x += move_x;
    this->professor_sprite_.move(move_x, 0.f);
}


void Professor::move(float background_movement, float background_location, sf::Vector2f player_position) {
    this->move_vertical();
    this->move_horizontal(background_movement, background_location);
    this->face_player(player_position);
 }


void Professor::render(sf::RenderTarget &target, float background_movement) {
    target.draw(this->professor_sprite_);
}


std::shared_ptr<Throwable> Professor::shoot_throwable(sf::Texture& texture, sf::Vector2f player_position) {

    //Shoot the professor assignment if he has cooled down after his throw
    if (this->current_cool_down < this->max_cool_down) return nullptr;
    this->current_cool_down = 0;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> cool_down_distribution(4000, 8000);
    this->max_cool_down = cool_down_distribution(generator);
    auto assignment = std::make_shared<Professor_Assignment>(texture, this->professor_sprite_.getPosition(), player_position, this->get_world_bounds());
    return assignment;
}


void Professor::increment_cool_down() {
    if (this->professor_sprite_.getGlobalBounds().left > 0 && this->professor_sprite_.getGlobalBounds().left < 1400.f) {
        this->current_cool_down++;
    }
}


void Professor::destroy() {

    //CHange the sprite shown to produce a dying animation
    if (this->is_dying_counter++ < 100)
        this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 870.f, 334.f, 275.f));
    else if (this->is_dying_counter++ < 500) {
        this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 461.f, 471.f));
    } else
        this->is_dead = true;
}


bool Professor::get_is_dead() { return this->is_dead; }


bool Professor::is_dying() { return this->is_dying_counter > 0; }


sf::FloatRect Professor::get_world_bounds() {
    // auto world_bounds = sf::FloatRect(this->world_position.x + 20.f, this->world_position.y + 5.f, this->professor_sprite_.getGlobalBounds().width - 40.f, this->professor_sprite_.getGlobalBounds().height - 10.f);
    auto world_bounds = sf::FloatRect(this->professor_sprite_.getPosition().x + 20.f, this->professor_sprite_.getPosition().y + 5.f, this->professor_sprite_.getGlobalBounds().width - 40.f, this->professor_sprite_.getGlobalBounds().height - 10.f);
    return world_bounds;
}


sf::Vector2f Professor::get_location() { return this->professor_sprite_.getPosition(); }


