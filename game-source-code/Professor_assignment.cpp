/** \file Professor_Assignment.cpp
    \brief Contains the implementation of the professor's assignment object
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include <iostream>
#include "Professor_Assignment.h"

//Constructor that sets up the angle and position of the assignment that is thrown
Professor_Assignment::Professor_Assignment(sf::Texture& texture, sf::Vector2f professor_location, sf::Vector2f player_location, sf::FloatRect professor_world_position) : 
    assignment_texture_(texture), initial_professor_location(professor_location), initial_player_location(player_location), world_bounds_{professor_world_position} {
    assignment_sprite_.setTexture(assignment_texture_);
    calculate_angle_of_assignment();
    assignment_sprite_.setRotation(angle_*(180/M_PI));
    assignment_sprite_.setScale(assignment_scale_, assignment_scale_);
    assignment_sprite_.setPosition(professor_location);
}


sf::Vector2f Professor_Assignment::get_location() {
    return assignment_sprite_.getPosition();
}


void Professor_Assignment::move(float background_movement) {

    //Calculates how much the assignment should move in the x and y directions
    float x_component = assignment_speed_*cos(angle_) + background_movement;
    float y_component = assignment_speed_*sin(angle_);

    assignment_sprite_.move(x_component, y_component);
    auto new_world_bounds = sf::FloatRect(world_bounds_.left+x_component, world_bounds_.top+y_component, world_bounds_.width, world_bounds_.height);
    world_bounds_ = new_world_bounds;
}


void Professor_Assignment::draw(sf::RenderTarget &target) {
    target.draw(assignment_sprite_);
}


void Professor_Assignment::calculate_angle_of_assignment () {

    //Use arctan to calculate the angle of the assignment throw
    float angle_radians = std::atan((initial_player_location.y+20 - initial_professor_location.y) / (initial_player_location.x+20 - initial_professor_location.x));
    if (get_relative_side() == -1) angle_radians += M_PI;
    angle_ = angle_radians;
}


int Professor_Assignment::get_relative_side () {
    if (initial_professor_location.x < initial_player_location.x) return 1;
    return -1;
}


sf::FloatRect Professor_Assignment::get_bounds() { return assignment_sprite_.getGlobalBounds(); }



sf::FloatRect Professor_Assignment::get_world_bounds() {
    return world_bounds_;
}


float Professor_Assignment::get_angle() { return angle_; }
