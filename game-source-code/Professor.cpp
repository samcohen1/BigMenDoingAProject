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

Professor::Professor(sf::Texture& texture) {
    num_professors_++;
    this->init_professor(texture);
    this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 488.f, 461.f, 380.f));
    this->professor_sprite_.setScale(this->scale_professor_,this->scale_professor_);
    this->professor_sprite_.setPosition(this->initial_x_position, this->initial_y_position); // for now just in window
}
int Professor::get_num_professors() { return num_professors_; }

void Professor::init_professor(sf::Texture& texture) {
    this->professor_sprite_.setTexture(texture);

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> amplitude_distributor(-400, 400);
    std::uniform_int_distribution<> period_distributor(10, 20);
    std::uniform_int_distribution<> gradient_distributor(-10, 10);
    this->movement_amplitude = amplitude_distributor(generator);
    this->movement_period = float(period_distributor(generator))/1000;
    this->movement_gradient = float(gradient_distributor(generator))/200;

    this->initial_x_position = 23+rand()%(1400-46);
    this->initial_y_position = 120+rand()%(630);
    this->world_position = this->initial_x_position;

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
    this->initial_x_position = this->world_position;
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

void Professor::move_professor(float background_location, sf::Vector2f player_position) {
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

     if(this->world_position <= -2775.f || this->world_position >= 4177.f - this->professor_sprite_.getGlobalBounds().width){
       this->flip_professor();
    }

    this->professor_sprite_.setPosition(this->initial_x_position + x + background_location, y);
    this->face_player(player_position);
}

void Professor::render_assignments(sf::RenderTarget &target, float background_movement) {
    for (auto i = 0; i < this->assignments_.size(); i++) {
        if(this->assignments_[i]->get_location().x > -700.f && this->assignments_[i]->get_location().x < 2100.f) {
            this->assignments_[i]->move_assignment(background_movement);
            this->assignments_[i]->draw_assignment(target);
        } else this->erase_assignment(i);
    }
}

void Professor::render(sf::RenderTarget &target, float background_movement) {
    this->render_assignments(target, background_movement);
    target.draw(this->professor_sprite_);
}

std::vector<std::shared_ptr<Professor_Assignment>> Professor::get_assignments() const { return this->assignments_; }

void Professor::shoot_assignment(sf::Texture& texture, sf::Vector2f player_position) {
    if(this->current_cool_down < this->max_cool_down) return;
    this-> current_cool_down = 0;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> cool_down_distribution(4000, 8000);
    this->max_cool_down = cool_down_distribution(generator);
    this->assignments_.push_back(std::make_shared<Professor_Assignment>(texture, this->professor_sprite_.getPosition(), player_position));
}

void Professor::increment_cool_down() {
    if (this->professor_sprite_.getGlobalBounds().left > 0 && this->professor_sprite_.getGlobalBounds().left < 1400.f) {
        this->current_cool_down++;
    }
}

void Professor::erase_assignment(int position) {
    this->assignments_.erase(this->assignments_.begin() + position);
}

sf::Vector2f Professor::get_location() {
    return this->professor_sprite_.getPosition();
}