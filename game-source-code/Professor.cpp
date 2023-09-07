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

Professor::Professor(sf::Texture& texture) {
    this->init_professor(texture);
    this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 488.f, 461.f, 380.f));
    this->professor_sprite_.setScale(this->scale_professor_,this->scale_professor_);
    this->professor_sprite_.setPosition(this->initial_x_position, this->initial_y_position); // for now just in window
}

void Professor::init_professor(sf::Texture& texture) {
    this->professor_sprite_.setTexture(texture);
    // constants
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> amplitude_distributor(-400, 400);
    std::uniform_int_distribution<> period_distributor(10, 20);
    std::uniform_int_distribution<> gradient_distributor(-5, 5);
    this->movement_amplitude = amplitude_distributor(generator);
    this->movement_period = float(period_distributor(generator))/1000;
    this->movement_gradient = float(gradient_distributor(generator))/1000;

    this->initial_x_position = 23+rand()%(1400-46);
    this->initial_y_position = 120+rand()%(630);
    this->world_position = this->initial_x_position;

    if (amplitude_distributor(generator) > 0) {
        this->horizontal_speed_ *= -1;
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
    this->initial_x_position = this->world_position;
    this->y_shift += this->movement_function();
    this->frames_since_spawn = 0;
}

bool Professor::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

void Professor::move_professor(float background_location) {
    this->frames_since_spawn++;
    float x = this->frames_since_spawn*this->horizontal_speed_;
    float y = this->initial_y_position + this->movement_function() + this->y_shift;
    this->world_position += this->horizontal_speed_;;
   
   // Keep Within Bounds
    if(y <= this->y_min_pos_){
        y = y_min_pos_ + (y_min_pos_-y);
    } else if (y >= y_max_pos_){
        y = y_max_pos_ - (y-y_max_pos_);
    }

     if(this->world_position <= -2750.f || this->world_position >= 4150.f){
       this->flip_professor();
    }

    this->professor_sprite_.setPosition(this->initial_x_position + x + background_location, y);
}

void Professor::edge_professor_movement(Direction direction) {
    

}

void Professor::edge_decelerate() {
    
}

void Professor::correct_edge_positions() {
    
}

void Professor::render_bullets(sf::RenderTarget &target, float background_movement) {
    
}

void Professor::render(sf::RenderTarget &target) {
    target.draw(this->professor_sprite_);
}

// float Professor::get_professor_speed() const { return this->professor_vertical_speed_*static_cast<float>(this->prev_direction_); }

// std::vector<std::shared_ptr<Bullet>> Professor::get_bullets() const { return this->bullets_; }

void Professor::shoot_bullet(sf::Texture& texture) {
    
}

void Professor::increment_cool_down() {
    
}

void Professor::erase_bullet(int position) {
    
}
