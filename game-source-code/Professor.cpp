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
    init_professor(texture);
    professor_sprite_.setTextureRect(sf::IntRect(0.f, 488.f, 461.f, 380.f));
    professor_sprite_.setScale(scale_professor_, scale_professor_);
    professor_sprite_.setPosition(initial_x_position, initial_y_position);
}


Professor::~Professor() {
    //Decrement static number of professors
    num_professors_--;
}


int Professor::get_num_professors() { return num_professors_; }


void Professor::init_professor(sf::Texture& texture) {
    professor_sprite_.setTexture(texture);

    //Generate and set the professors movement function which is a sine wave with a differing period, amplitude and gradient-shift
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> amplitude_distributor(-400, 400);
    std::uniform_int_distribution<> period_distributor(10, 20);
    std::uniform_int_distribution<> gradient_distributor(-10, 10);
    std::uniform_int_distribution<> initial_x_distributor(-2777, 5650);
    std::uniform_int_distribution<> initial_y_distributor(120, 750);

    movement_amplitude = amplitude_distributor(generator);
    movement_period = float(period_distributor(generator)) / 1000;
    movement_gradient = float(gradient_distributor(generator)) / 200;

    // Initialize world position to the starting position.
    world_position.x = initial_x_distributor(generator);
    world_position.y = initial_y_distributor(generator);
    initial_x_position = world_position.x;
    initial_y_position = world_position.y;
    prev_y_position = world_position.y;

    int choose_direction = amplitude_distributor(generator);
    if (choose_direction > 0) {
        horizontal_speed_ *= -1;
        flip_professor();
    } else
        flip_professor();
}


float Professor::movement_function() {
    //Calculate the professor's y-value using the number of frames since he was spawned
    float x = frames_since_spawn * vertical_speed_;
    return movement_amplitude * std::sin(movement_period * x) + movement_gradient * x;
}

void Professor::flip_professor() {
    //Turn the professor around when applicable and make him move in the other direction
    horizontal_speed_ *= -1.f;
    int prev_direction_flag = horizontal_speed_ >= 0 ? 1 : -1;
    professor_sprite_.setScale(-prev_direction_flag * scale_professor_, scale_professor_);
    professor_sprite_.setOrigin(((prev_direction_flag + 1) / 2.f) * professor_sprite_.getGlobalBounds().width / scale_professor_, 0.f);
    initial_x_position = world_position.x;
    frames_since_spawn = 0;
}


void Professor::face_player(sf::Vector2f player_position) {

    //Turn the professor around to face the player
    if (professor_sprite_.getPosition().x < player_position.x) {
        professor_sprite_.setScale(-scale_professor_, scale_professor_);
        professor_sprite_.setOrigin(professor_sprite_.getGlobalBounds().width / scale_professor_, 0.f);
    } else {
        professor_sprite_.setScale(scale_professor_, scale_professor_);
        professor_sprite_.setOrigin(0.f, 0.f);
    }
}


bool Professor::direction_changed(Direction direction) { return prev_direction_ != direction; }

void Professor::move_vertical () {

    //Check the total y-position and the difference from the previous y-position to know how much the professor should move
    frames_since_spawn++;
    int y = static_cast<int>(initial_y_position + movement_function());
    float move_y = y-prev_y_position;

    //Ensure the professor stays within bounds
    auto temp_pos = world_position.y + move_y;
    if (temp_pos <= y_min_pos_)  {
        if(move_y < 0) y_direction = -1;
        else y_direction = 1;
    }
    else if (temp_pos + professor_sprite_.getGlobalBounds().height >= y_max_pos_) {
        if(move_y > 0) y_direction = -1;
        else y_direction = 1;
    }
    world_position.y += y_direction*move_y;
    professor_sprite_.move(0.f, y_direction*move_y);
    prev_y_position = y;
}

void Professor::move_horizontal (float background_movement, float background_location) {

    //Calculate how much the professor should move
    float move_x = horizontal_speed_ + background_movement;
    auto temp_pos = world_position.x + move_x;

    //Ensure the professor remains in bounds
    if (temp_pos <= -2800+professor_sprite_.getGlobalBounds().width) move_x += 8500.f-1400.f;
    else if (temp_pos >= 5700-professor_sprite_.getGlobalBounds().width) move_x -= 8500.f-1400.f;
    world_position.x += move_x;
    professor_sprite_.move(move_x, 0.f);
}


void Professor::move(float background_movement, float background_location, sf::Vector2f player_position) {
    move_vertical();
    move_horizontal(background_movement, background_location);
    face_player(player_position);
 }


void Professor::render(sf::RenderTarget &target, float background_movement) {
    target.draw(professor_sprite_);
}


std::shared_ptr<Throwable> Professor::shoot_throwable(sf::Texture& texture, sf::Vector2f player_position) {

    //Shoot the professor assignment if he has cooled down after his throw
    if (current_cool_down < max_cool_down) return nullptr;
    current_cool_down = 0;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> cool_down_distribution(4000, 8000);
    max_cool_down = cool_down_distribution(generator);
    auto assignment = std::make_shared<Professor_Assignment>(texture, professor_sprite_.getPosition(), player_position, get_world_bounds());
    return assignment;
}


void Professor::increment_cool_down() {
    if (professor_sprite_.getGlobalBounds().left > 0 && professor_sprite_.getGlobalBounds().left < 1400.f) {
        current_cool_down++;
    }
}


void Professor::destroy() {

    //CHange the sprite shown to produce a dying animation
    if (is_dying_counter++ < 100)
        professor_sprite_.setTextureRect(sf::IntRect(0.f, 870.f, 334.f, 275.f));
    else if (is_dying_counter++ < 500) {
        professor_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 461.f, 471.f));
    } else
        is_dead = true;
}


bool Professor::get_is_dead() { return is_dead; }


bool Professor::is_dying() { return is_dying_counter > 0; }


sf::FloatRect Professor::get_world_bounds() {
    // auto world_bounds = sf::FloatRect(world_position.x + 20.f, world_position.y + 5.f, professor_sprite_.getGlobalBounds().width - 40.f, professor_sprite_.getGlobalBounds().height - 10.f);
    auto world_bounds = sf::FloatRect(professor_sprite_.getPosition().x + 20.f, professor_sprite_.getPosition().y + 5.f, professor_sprite_.getGlobalBounds().width - 40.f, professor_sprite_.getGlobalBounds().height - 10.f);
    return world_bounds;
}


sf::Vector2f Professor::get_location() { return professor_sprite_.getPosition(); }


