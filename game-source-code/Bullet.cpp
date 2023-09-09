#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <memory>

Bullet::Bullet(float x_location, float y_location, Direction direction, sf::Texture& texture) : bullet_texture_(texture), x_location_(x_location), y_location_(y_location), direction_(direction) {
    this->bullet_sprite_.setTexture(this->bullet_texture_);
    this->bullet_sprite_.setScale(static_cast<float>(direction) * this->bullet_scale_, this->bullet_scale_);
    this->bullet_sprite_.setOrigin(((static_cast<float>(direction)+1)/2.f)*this->bullet_sprite_.getGlobalBounds().width/this->bullet_scale_, 0.f);
    this->bullet_sprite_.setPosition(x_location, y_location);
    this->world_position = this->bullet_sprite_.getPosition();
}

void Bullet::move(float background_movement) {
    this->bullet_sprite_.move(static_cast<float>(this->direction_) * this->bullet_movement, 0.f);
    x_location_ += (static_cast<float>(this->direction_)*this->bullet_movement) - background_movement;
    this->world_position = this->bullet_sprite_.getPosition();
}

void Bullet::draw(sf::RenderTarget& target) {
    target.draw(this->bullet_sprite_);
}

sf::Vector2f Bullet::get_location() { return this->bullet_sprite_.getPosition(); }
sf::FloatRect Bullet::get_bounds() { return this->bullet_sprite_.getGlobalBounds(); }
sf::FloatRect Bullet::get_world_bounds() { 
    auto world_bounds = sf::FloatRect(this->world_position.x, this->world_position.y, this->bullet_sprite_.getGlobalBounds().width, this->bullet_sprite_.getGlobalBounds().height);
    return world_bounds; 
}
