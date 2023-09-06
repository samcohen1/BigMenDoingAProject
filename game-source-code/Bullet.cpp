#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <memory>

Bullet::Bullet(float x_location, float y_location, Direction direction) : x_location_(x_location), y_location_(y_location) {
    if(!this->bullet_texture_.loadFromFile("resources/Bullet.PNG")) return;
    this->bullet_sprite_.setTexture(this->bullet_texture_);
    this->bullet_sprite_.setScale(static_cast<float>(direction) * this->bullet_scale_, this->bullet_scale_);
    this->bullet_sprite_.setOrigin(((static_cast<float>(direction)+1)/2.f)*this->bullet_sprite_.getGlobalBounds().width/this->bullet_scale_, 0.f);
    this->bullet_sprite_.setPosition(x_location, y_location);
}

void Bullet::move_bullet(float background_movement) {

}

void Bullet::draw_bullet(sf::RenderTarget& target) {
    target.draw(this->bullet_sprite_);
}

sf::Vector2f Bullet::get_location() {
    return this->bullet_sprite_.getPosition();
}
