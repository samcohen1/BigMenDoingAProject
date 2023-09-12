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

/** \class Professor
 *  \brief Represents a Professor character in the game.
 */
Professor::Professor() {}

/** \class Professor
 *  \brief Represents a Professor character in the game.
 *  \param texture The texture for the Professor's sprite.
 *  This constructor initializes a Professor character with the given texture.
 */
Professor::Professor(sf::Texture& texture) {
    num_professors_++;
    this->init_professor(texture);
    this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 488.f, 461.f, 380.f));
    this->professor_sprite_.setScale(this->scale_professor_, this->scale_professor_);
    this->professor_sprite_.setPosition(this->initial_x_position, this->initial_y_position);
}

/** \class Professor
 *  \brief Destructor for the Professor class.
 */
Professor::~Professor() {
    num_professors_--;
}

/** \fn int Professor::get_num_professors()
 *  \brief Get the number of Professor objects.
 *  \return The number of Professor objects.
 */
int Professor::get_num_professors() { return num_professors_; }

/** \fn void Professor::init_professor(sf::Texture& texture)
 *  \brief Initialize the Professor object.
 *  \param texture The texture for the Professor's sprite.
 *  This function sets up the Professor's initial properties, including its world position.
 */
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
    this->movement_period = float(period_distributor(generator)) / 1000;
    this->movement_gradient = float(gradient_distributor(generator)) / 200;

    // Initialize world position to the starting position.
    this->world_position.x = initial_x_distributor(generator);
    this->world_position.y = initial_y_distributor(generator);

    int choose_direction = amplitude_distributor(generator);
    if (choose_direction > 0) {
        this->horizontal_speed_ *= -1;
        this->flip_professor();
    } else
        this->flip_professor();
}

/** \fn float Professor::movement_function()
 *  \brief Calculate the movement function for the Professor.
 *  \return The calculated movement value.
 *  This function calculates the vertical movement of the Professor character based on amplitude, period, and gradient.
 */
float Professor::movement_function() {
    float x = this->frames_since_spawn * this->vertical_speed_;
    return this->movement_amplitude * std::sin(this->movement_period * x) + this->movement_gradient * x;
}

/** \fn void Professor::flip_professor()
 *  \brief Flip the Professor's sprite to change direction.
 *  This function flips the Professor's sprite horizontally to change its direction of movement.
 */
void Professor::flip_professor() {
    this->horizontal_speed_ *= -1.f;
    int prev_direction_flag = this->horizontal_speed_ >= 0 ? 1 : -1;
    this->professor_sprite_.setScale(-prev_direction_flag * this->scale_professor_, this->scale_professor_);
    this->professor_sprite_.setOrigin(((prev_direction_flag + 1) / 2.f) * this->professor_sprite_.getGlobalBounds().width / this->scale_professor_, 0.f);
    this->initial_x_position = this->world_position.x;
    this->y_shift += this->movement_function();
    this->frames_since_spawn = 0;
}

/** \fn void Professor::face_player(sf::Vector2f player_position)
 *  \brief Make the Professor face the player character.
 *  \param player_position The position of the player character.
 *  This function flips the Professor's sprite to make it face the player character.
 */
void Professor::face_player(sf::Vector2f player_position) {
    if (this->professor_sprite_.getGlobalBounds().left < player_position.x) {
        this->professor_sprite_.setScale(-this->scale_professor_, this->scale_professor_);
        this->professor_sprite_.setOrigin(this->professor_sprite_.getGlobalBounds().width / this->scale_professor_, 0.f);
    } else {
        this->professor_sprite_.setScale(this->scale_professor_, this->scale_professor_);
        this->professor_sprite_.setOrigin(0.f, 0.f);
    }
}

/** \fn bool Professor::direction_changed(Direction direction)
 *  \brief Check if the direction of the Professor has changed.
 *  \param direction The new direction to check against.
 *  \return True if the direction has changed; false otherwise.
 *  This function checks if the direction of the Professor has changed compared to the provided direction.
 */
bool Professor::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

/** \fn void Professor::move(float background_location, sf::Vector2f player_position)
 *  \brief Move the Professor character.
 *  \param background_location The location of the game background.
 *  \param player_position The position of the player character.
 *  This function updates the position of the Professor character based on its movement properties and player position.
 */
void Professor::move(float background_location, sf::Vector2f player_position) {
    this->frames_since_spawn++;
    float x = this->frames_since_spawn * this->horizontal_speed_;
    float y = this->initial_y_position + this->movement_function() + this->y_shift;
    this->world_position.x += this->horizontal_speed_;

    // Keep Within Bounds
    if (y <= this->y_min_pos_) {
        y = y_min_pos_ + (y_min_pos_ - y);
    } else if (y >= y_max_pos_) {
        y = y_max_pos_ - (y - y_max_pos_);
    }

    if (this->world_position.x <= -2775.f || this->world_position.x >= 4177.f - this->professor_sprite_.getGlobalBounds().width) {
        this->flip_professor();
    }

    this->world_position.y = y;
    this->professor_sprite_.setPosition(this->initial_x_position + x + background_location, y);
    this->face_player(player_position);
}

/** \fn void Professor::render(sf::RenderTarget &target, float background_movement)
 *  \brief Render the Professor on the game screen.
 *  \param target The render target to draw on.
 *  \param background_movement The movement of the game background.
 *  This function renders the Professor character on the game screen.
 */
void Professor::render(sf::RenderTarget &target, float background_movement) {
    target.draw(this->professor_sprite_);
}

/** \fn std::shared_ptr<Throwable> Professor::shoot_throwable(sf::Texture& texture, sf::Vector2f player_position)
 *  \brief Create and return a Throwable object.
 *  \param texture The texture for the Throwable.
 *  \param player_position The position of the player character.
 *  \return A shared pointer to the created Throwable object.
 *  This function creates and returns a Throwable object, representing an attack by the Professor character.
 */
std::shared_ptr<Throwable> Professor::shoot_throwable(sf::Texture& texture, sf::Vector2f player_position) {
    if (this->current_cool_down < this->max_cool_down) return nullptr;
    this->current_cool_down = 0;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> cool_down_distribution(4000, 8000);
    this->max_cool_down = cool_down_distribution(generator);
    auto assignment = std::make_shared<Professor_Assignment>(texture, this->professor_sprite_.getPosition(), player_position, this->get_world_bounds());
    return assignment;
}

// ...

/** \fn void Professor::increment_cool_down()
 *  \brief Increment the cooldown for the Professor's attacks.
 *  This function increments the cooldown timer for the Professor's attacks.
 */
void Professor::increment_cool_down() {
    if (this->professor_sprite_.getGlobalBounds().left > 0 && this->professor_sprite_.getGlobalBounds().left < 1400.f) {
        this->current_cool_down++;
    }
}

/** \fn void Professor::destroy()
 *  \brief Destroy the Professor character.
 *  This function handles the destruction of the Professor character, including the sprite animation.
 */
void Professor::destroy() {
    if (this->is_dying_counter++ < 100)
        this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 870.f, 334.f, 275.f));
    else if (this->is_dying_counter++ < 500) {
        this->professor_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 461.f, 471.f));
    } else
        this->is_dead = true;
}

/** \fn bool Professor::get_is_dead()
 *  \brief Check if the Professor character is dead.
 *  \return True if the Professor character is dead; false otherwise.
 *  This function checks whether the Professor character is dead.
 */
bool Professor::get_is_dead() { return this->is_dead; }

/** \fn bool Professor::is_dying()
 *  \brief Check if the Professor character is in the process of dying.
 *  \return True if the Professor character is in the process of dying; false otherwise.
 *  This function checks if the Professor character is in the process of dying.
 */
bool Professor::is_dying() { return this->is_dying_counter > 0; }

/** \fn sf::FloatRect Professor::get_world_bounds()
 *  \brief Get the bounding box of the Professor in the game world.
 *  \return The bounding box of the Professor in the game world.
 *  This function returns the bounding box of the Professor character in the game world.
 */
sf::FloatRect Professor::get_world_bounds() {
    auto world_bounds = sf::FloatRect(this->world_position.x + 20.f, this->world_position.y + 5.f, this->professor_sprite_.getGlobalBounds().width - 40.f, this->professor_sprite_.getGlobalBounds().height - 10.f);
    return world_bounds;
}

/** \fn sf::Vector2f Professor::get_location()
 *  \brief Get the current location of the Professor character.
 *  \return The current location of the Professor character.
 *  This function returns the current location of the Professor character.
 */
sf::Vector2f Professor::get_location() { return this->professor_sprite_.getPosition(); }

/** \fn sf::FloatRect Professor::get_bounds()
 *  \brief Get the bounding box of the Professor's sprite.
 *  \return The bounding box of the Professor's sprite.
 *  This function returns the bounding box of the Professor's sprite, excluding a margin.
 */
sf::FloatRect Professor::get_bounds() {
    auto global = this->professor_sprite_.getGlobalBounds();
    auto tight_bounds = sf::FloatRect(global.left + 20.f, global.top, global.width - 40.f, global.height);
    return tight_bounds;
}


