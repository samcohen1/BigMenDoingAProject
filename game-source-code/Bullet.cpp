/** \file Bullet.cpp
    \brief Contains the implementation of the player's bullet
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "Bullet.h"

Bullet::Bullet(float x_location, float y_location, Direction direction, sf::Texture& texture, sf::FloatRect player_world_position) : 
    bullet_texture_(texture), x_location_(x_location), y_location_(y_location), world_bounds_(player_world_position), direction_(direction) {
    bullet_sprite_.setTexture(bullet_texture_);
    bullet_sprite_.setScale(static_cast<float>(direction) * bullet_scale_*0.5f, bullet_scale_);
    bullet_sprite_.setOrigin(((static_cast<float>(direction)+1)/2.f)*bullet_sprite_.getGlobalBounds().width/bullet_scale_, 0.f);
    bullet_sprite_.setPosition(x_location, y_location);
}

void Bullet::move(float background_movement) {
    float x_component = static_cast<float>(direction_) * bullet_movement;
    bullet_sprite_.move(x_component + background_movement, 0.f);
    x_location_ += x_component + background_movement;

    auto new_world_bounds = sf::FloatRect(world_bounds_.left+x_component, world_bounds_.top, world_bounds_.width, world_bounds_.height);
    world_bounds_ = new_world_bounds;
}

void Bullet::draw(sf::RenderTarget& target) {
    target.draw(bullet_sprite_);
}

sf::Vector2f Bullet::get_location() { 
    return bullet_sprite_.getPosition(); 
}

sf::FloatRect Bullet::get_world_bounds() { 
    auto pos = bullet_sprite_.getPosition();
    return sf::FloatRect(pos.x, pos.y, bullet_sprite_.getGlobalBounds().width, bullet_sprite_.getGlobalBounds().height);
}