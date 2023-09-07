#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include "Professor_Assignment.h"


Professor_Assignment::Professor_Assignment(sf::Texture& texture, sf::Vector2f professor_location, sf::Vector2f player_location) :
    assignment_texture_(texture), initial_professor_location(professor_location), initial_player_location(player_location)
{
    this->assignment_sprite_.setTexture(this->assignment_texture_);
    this->assignment_sprite_.setRotation(this->angle_of_assignment());
    this->assignment_sprite_.setScale(this->assignment_scale_, this->assignment_scale_);
    this->assignment_sprite_.setPosition(professor_location);
}

sf::Vector2f Professor_Assignment::get_location() {
    return assignment_sprite_.getPosition();
}

void Professor_Assignment::move_assignment(float background_movement) {
    
}

void Professor_Assignment::draw_assignment(sf::RenderTarget &target) {
    target.draw(this->assignment_sprite_);
}

float Professor_Assignment::angle_of_assignment() {
    return std::atan((initial_player_location.y - initial_professor_location.y) / (initial_player_location.x - initial_professor_location.x));
}
