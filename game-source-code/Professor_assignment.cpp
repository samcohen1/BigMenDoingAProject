#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include <iostream>
#include "Professor_Assignment.h"


Professor_Assignment::Professor_Assignment(sf::Texture& texture, sf::Vector2f professor_location, sf::Vector2f player_location) :
    assignment_texture_(texture), initial_professor_location(professor_location), initial_player_location(player_location) {
    this->assignment_sprite_.setTexture(this->assignment_texture_);
    this->calculate_angle_of_assignment();
    this->assignment_sprite_.setRotation(this->angle_*(180/M_PI));
    this->assignment_sprite_.setScale(this->assignment_scale_, this->assignment_scale_);
    this->assignment_sprite_.setPosition(professor_location);
    this->world_position = this->assignment_sprite_.getPosition();
}

sf::Vector2f Professor_Assignment::get_location() {
    return assignment_sprite_.getPosition();
}

void Professor_Assignment::move(float background_movement) {
    float x_component = this->assignment_speed_*cos(this->angle_);
    float y_component = this->assignment_speed_*sin(this->angle_);

    this->assignment_sprite_.move(x_component + background_movement, y_component);
    this->world_position = this->assignment_sprite_.getPosition();
}

void Professor_Assignment::draw(sf::RenderTarget &target) {
    target.draw(this->assignment_sprite_);
}

void Professor_Assignment::calculate_angle_of_assignment () {
    float angle_radians = std::atan((initial_player_location.y+20 - initial_professor_location.y) / (initial_player_location.x+20 - initial_professor_location.x));
    if (this->get_relative_side() == -1) angle_radians += M_PI;
    this->angle_ = angle_radians;
}

int Professor_Assignment::get_relative_side () {
    if (this->initial_professor_location.x < this->initial_player_location.x) return 1;
    return -1;
}

sf::FloatRect Professor_Assignment::get_bounds() { return this->assignment_sprite_.getGlobalBounds(); }
sf::FloatRect Professor_Assignment::get_world_bounds() { 
    auto world_bounds = sf::FloatRect(this->world_position.x, this->world_position.y, this->assignment_sprite_.getGlobalBounds().width, this->assignment_sprite_.getGlobalBounds().height);
    return world_bounds; 
}