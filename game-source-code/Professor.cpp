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

Professor::Professor() {}

Professor::Professor(sf::Texture& texture) {
    num_professors_++;
    this->init_professor(texture);
    this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 488.f, 461.f, 380.f));
    this->professor_sprite_.setScale(this->scale_professor_,this->scale_professor_);
    this->professor_sprite_.setPosition(this->initial_x_position, this->initial_y_position);
}
Professor::~Professor() {
    num_professors_--;
}
int Professor::get_num_professors() { return num_professors_; }

void Professor::init_professor(sf::Texture& texture) {
    this->professor_sprite_.setTexture(texture);

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> amplitude_distributor(-400, 400);
    std::uniform_int_distribution<> period_distributor(10, 20);
    std::uniform_int_distribution<> gradient_distributor(-10, 10);
    std::uniform_int_distribution<> initial_x_distributor(-2777, 2777);
    std::uniform_int_distribution<> initial_y_distributor(120, 750);

    this->movement_amplitude = amplitude_distributor(generator);
    this->movement_period = float(period_distributor(generator))/1000;
    this->movement_gradient = float(gradient_distributor(generator))/200;

    this->world_position.x = initial_x_distributor(generator);
    this->world_position.y = initial_y_distributor(generator);

    int choose_direction = amplitude_distributor(generator);
    if (choose_direction > 0) {
        this->horizontal_speed_ *= -1;
        this->flip_professor();
    } else this->flip_professor();
}

float Professor::movement_function() {
    float x = this->frames_since_spawn*this->vertical_speed_;
    return this->movement_amplitude*std::sin(this->movement_period*x) + this->movement_gradient*x;
}

void Professor::flip_professor() {
    this->horizontal_speed_ *= -1.f;
    int prev_direction_flag = this->horizontal_speed_ >= 0 ? 1 : -1;
    this->professor_sprite_.setScale(-prev_direction_flag*this->scale_professor_, this->scale_professor_);
    this->professor_sprite_.setOrigin(((prev_direction_flag+1)/2.f)*this->professor_sprite_.getGlobalBounds().width/this->scale_professor_, 0.f);
    this->initial_x_position = this->world_position.x;
    this->y_shift += this->movement_function();
    this->frames_since_spawn = 0;
}

void Professor::face_player (sf::Vector2f player_position) {
    if (this->professor_sprite_.getGlobalBounds().left < player_position.x) {
        this->professor_sprite_.setScale(-this->scale_professor_, this->scale_professor_);
        this->professor_sprite_.setOrigin(this->professor_sprite_.getGlobalBounds().width/this->scale_professor_, 0.f);
    } else {
        this->professor_sprite_.setScale(this->scale_professor_, this->scale_professor_);
        this->professor_sprite_.setOrigin(0.f, 0.f);
    }
}

bool Professor::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

void Professor::move(float background_location, sf::Vector2f player_position) {
    this->frames_since_spawn++;
    float x = this->frames_since_spawn*this->horizontal_speed_;
    float y = this->initial_y_position + this->movement_function() + this->y_shift;
    this->world_position.x += this->horizontal_speed_;
   
   // Keep Within Bounds
    if(y <= this->y_min_pos_){
        y = y_min_pos_ + (y_min_pos_-y);
    } else if (y >= y_max_pos_){
        y = y_max_pos_ - (y-y_max_pos_);
    }

     if(this->world_position.x <= -2775.f || this->world_position.x >= 4177.f - this->professor_sprite_.getGlobalBounds().width){
       this->flip_professor();
    }

    this->world_position.y = y;
    this->professor_sprite_.setPosition(this->initial_x_position + x + background_location, y);
    this->face_player(player_position);
}

void Professor::render(sf::RenderTarget &target, float background_movement) {
    target.draw(this->professor_sprite_);
}

std::shared_ptr<Throwable> Professor::shoot_throwable(sf::Texture& texture, sf::Vector2f player_position) {
    if(this->current_cool_down < this->max_cool_down) return nullptr;
    this-> current_cool_down = 0;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> cool_down_distribution(4000, 8000);
    this->max_cool_down = cool_down_distribution(generator);
    auto assigment = std::make_shared<Professor_Assignment>(texture, this->professor_sprite_.getPosition(), player_position);
    return assigment;
}

void Professor::increment_cool_down() {
    if (this->professor_sprite_.getGlobalBounds().left > 0 && this->professor_sprite_.getGlobalBounds().left < 1400.f) {
        this->current_cool_down++;
    }
}

void Professor::destroy() {
    if (this->is_dying_counter++ < 100) this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 870.f, 334.f, 275.f));
    else if (this->is_dying_counter++ < 500) { 
        this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 461.f, 471.f)); 
    } else {
        this->is_dead = true;
        this->is_dying_counter = 0;
    }
}
bool Professor::get_is_dead () { return this->is_dead; }
bool Professor::is_dying() {return this->is_dying_counter > 0; }

sf::FloatRect Professor::get_world_bounds() { 
    auto world_bounds = sf::FloatRect(this->world_position.x, this->world_position.y, this->professor_sprite_.getGlobalBounds().width, this->professor_sprite_.getGlobalBounds().height);
    return world_bounds; 
}
sf::Vector2f Professor::get_location() { return this->professor_sprite_.getPosition(); }

sf::FloatRect Professor::get_bounds() {
    auto global = this->professor_sprite_.getGlobalBounds();
    auto tight_bounds = sf::FloatRect(global.left+20.f, global.top, global.width-40.f, global.height);
    return tight_bounds;
}