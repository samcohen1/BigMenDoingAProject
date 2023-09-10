#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

Bullet::Bullet(float x_location, float y_location, Direction direction, sf::Texture& texture, sf::FloatRect player_world_position) : 
    bullet_texture_(texture), x_location_(x_location), y_location_(y_location), world_bounds_(player_world_position), direction_(direction) {
    this->bullet_sprite_.setTexture(this->bullet_texture_);
    this->bullet_sprite_.setScale(static_cast<float>(direction) * this->bullet_scale_, this->bullet_scale_);
    this->bullet_sprite_.setOrigin(((static_cast<float>(direction)+1)/2.f)*this->bullet_sprite_.getGlobalBounds().width/this->bullet_scale_, 0.f);
    this->bullet_sprite_.setPosition(x_location, y_location);
}

void Bullet::move(float background_movement) {
    float x_component = static_cast<float>(this->direction_) * this->bullet_movement;
    this->bullet_sprite_.move(x_component, 0.f);
    x_location_ += x_component - background_movement;

    auto new_world_bounds = sf::FloatRect(this->world_bounds_.left+x_component, this->world_bounds_.top, this->world_bounds_.width, this->world_bounds_.height);
    this->world_bounds_ = new_world_bounds;
}

void Bullet::draw(sf::RenderTarget& target) {
    target.draw(this->bullet_sprite_);
}

sf::Vector2f Bullet::get_location() { return this->bullet_sprite_.getPosition(); }
sf::FloatRect Bullet::get_bounds() { return this->bullet_sprite_.getGlobalBounds(); }
sf::FloatRect Bullet::get_world_bounds() { 
    return world_bounds_;
}
