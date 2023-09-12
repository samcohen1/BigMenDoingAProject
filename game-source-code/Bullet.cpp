#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

/**
 * \fn Bullet::Bullet(float x_location, float y_location, Direction direction, sf::Texture& texture, sf::FloatRect player_world_position)
 * \brief This constructor initializes a Bullet object with the specified location, direction, texture, and world position.
 * 
 * \param x_location The initial x-coordinate of the bullet.
 * \param y_location The initial y-coordinate of the bullet.
 * \param direction The direction the bullet is fired in.
 * \param texture The texture applied to the bullet sprite.
 * \param player_world_position The world boundaries within which the bullet operates.
 * 
 */
Bullet::Bullet(float x_location, float y_location, Direction direction, sf::Texture& texture, sf::FloatRect player_world_position) : 
    bullet_texture_(texture), x_location_(x_location), y_location_(y_location), world_bounds_(player_world_position), direction_(direction) {
    this->bullet_sprite_.setTexture(this->bullet_texture_);
    this->bullet_sprite_.setScale(static_cast<float>(direction) * this->bullet_scale_, this->bullet_scale_);
    this->bullet_sprite_.setOrigin(((static_cast<float>(direction)+1)/2.f)*this->bullet_sprite_.getGlobalBounds().width/this->bullet_scale_, 0.f);
    this->bullet_sprite_.setPosition(x_location, y_location);
}

/**
 * \fn void Bullet::move(float background_movement)
 * \brief This method moves the bullet sprite based on its direction and adjusts its world bounds accordingly.
 * 
 * \param background_movement The amount of movement to consider for the background.
 * 
 */
void Bullet::move(float background_movement) {
    float x_component = static_cast<float>(this->direction_) * this->bullet_movement;
    this->bullet_sprite_.move(x_component, 0.f);
    x_location_ += x_component - background_movement;

    auto new_world_bounds = sf::FloatRect(this->world_bounds_.left+x_component, this->world_bounds_.top, this->world_bounds_.width, this->world_bounds_.height);
    this->world_bounds_ = new_world_bounds;
}

/**
 * \fn void Bullet::draw(sf::RenderTarget& target)
 * \brief This method draws the bullet sprite onto the target render target.
 * 
 * \param target The render target to which the bullet sprite is drawn.
 * 
 */
void Bullet::draw(sf::RenderTarget& target) {
    target.draw(this->bullet_sprite_);
}

/**
 * \fn sf::Vector2f Bullet::get_location()
 * \brief This method retrieves the current position of the bullet sprite.
 * 
 * \return Returns the current position of the bullet sprite as a vector of floats.
 * 
 */
sf::Vector2f Bullet::get_location() { return this->bullet_sprite_.getPosition(); }

/**
 * \fn sf::FloatRect Bullet::get_bounds()
 * \brief This method retrieves the global bounds of the bullet sprite.
 * 
 * \return Returns the global bounds of the bullet sprite as a floating point rectangle.
 * 
 */
sf::FloatRect Bullet::get_bounds() { return this->bullet_sprite_.getGlobalBounds(); }

/**
 * \fn sf::FloatRect Bullet::get_world_bounds()
 * \brief This method retrieves the current world bounds within which the bullet operates.
 * 
 * \return Returns the current world bounds as a floating point rectangle.
 * 
 */
sf::FloatRect Bullet::get_world_bounds() { 
    return world_bounds_;
}
