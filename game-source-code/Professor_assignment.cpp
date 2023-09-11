/** \class Professor_Assignment
 *  \brief Represents a professor's assignment object.
 * 
 *  This class represents a professor's assignment object, which is a projectile fired by the professor character.
 *  It manages the assignment's movement, rendering, and other properties.
 */
#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include <iostream>
#include "Professor_Assignment.h"

/** \fn Professor_Assignment::Professor_Assignment(sf::Texture& texture, sf::Vector2f professor_location, sf::Vector2f player_location, sf::FloatRect professor_world_position)
 *  \brief Constructor for Professor_Assignment.
 *  \param texture The texture for the assignment.
 *  \param professor_location The initial position of the professor character.
 *  \param player_location The initial position of the player character.
 *  \param professor_world_position The world bounds of the professor character.
 *  This constructor initializes a Professor_Assignment object with the provided parameters, setting its texture, position, and angle.
 */
Professor_Assignment::Professor_Assignment(sf::Texture& texture, sf::Vector2f professor_location, sf::Vector2f player_location, sf::FloatRect professor_world_position) : 
    assignment_texture_(texture), initial_professor_location(professor_location), initial_player_location(player_location), world_bounds_{professor_world_position} {
    this->assignment_sprite_.setTexture(this->assignment_texture_);
    this->calculate_angle_of_assignment();
    this->assignment_sprite_.setRotation(this->angle_*(180/M_PI));
    this->assignment_sprite_.setScale(this->assignment_scale_, this->assignment_scale_);
    this->assignment_sprite_.setPosition(professor_location);
}

/** \fn sf::Vector2f Professor_Assignment::get_location()
 *  \brief Get the current position of the assignment.
 *  \return The current position as an SFML Vector2f.
 *  This function returns the current position of the assignment.
 */
sf::Vector2f Professor_Assignment::get_location() {
    return assignment_sprite_.getPosition();
}

/** \fn void Professor_Assignment::move(float background_movement)
 *  \brief Move the assignment based on its angle and background movement.
 *  \param background_movement The movement of the game background.
 *  This function updates the position of the assignment based on its angle and background movement.
 */
void Professor_Assignment::move(float background_movement) {
    float x_component = this->assignment_speed_*cos(this->angle_) + background_movement;
    float y_component = this->assignment_speed_*sin(this->angle_);

    this->assignment_sprite_.move(x_component, y_component);
    auto new_world_bounds = sf::FloatRect(this->world_bounds_.left+x_component, this->world_bounds_.top+y_component, this->world_bounds_.width, this->world_bounds_.height);
    this->world_bounds_ = new_world_bounds;
}

/** \fn void Professor_Assignment::draw(sf::RenderTarget &target)
 *  \brief Draw the assignment on the screen.
 *  \param target The render target to draw on.
 *  This function draws the assignment on the provided render target.
 */
void Professor_Assignment::draw(sf::RenderTarget &target) {
    target.draw(this->assignment_sprite_);
}

/** \fn void Professor_Assignment::calculate_angle_of_assignment()
 *  \brief Calculate the angle of the assignment based on its initial positions.
 *  This function calculates the angle of the assignment based on its initial positions, ensuring it points towards the player character.
 */
void Professor_Assignment::calculate_angle_of_assignment () {
    float angle_radians = std::atan((initial_player_location.y+20 - initial_professor_location.y) / (initial_player_location.x+20 - initial_professor_location.x));
    if (this->get_relative_side() == -1) angle_radians += M_PI;
    this->angle_ = angle_radians;
}

/** \fn int Professor_Assignment::get_relative_side ()
 *  \brief Get the relative side of the assignment compared to the player character.
 *  \return 1 if the assignment is on the right side of the player, -1 if on the left.
 *  This function determines whether the assignment is on the right or left side of the player character.
 */
int Professor_Assignment::get_relative_side () {
    if (this->initial_professor_location.x < this->initial_player_location.x) return 1;
    return -1;
}

/** \fn sf::FloatRect Professor_Assignment::get_bounds()
 *  \brief Get the global bounds of the assignment sprite.
 *  \return The global bounds as an SFML FloatRect.
 *  This function returns the global bounds of the assignment sprite.
 */
sf::FloatRect Professor_Assignment::get_bounds() { return this->assignment_sprite_.getGlobalBounds(); }

/** \fn sf::FloatRect Professor_Assignment::get_world_bounds()
 *  \brief Get the world bounds of the assignment.
 *  \return The world bounds as an SFML FloatRect.
 *  This function returns the world bounds of the assignment.
 */
sf::FloatRect Professor_Assignment::get_world_bounds() {
    return this->world_bounds_;
}
