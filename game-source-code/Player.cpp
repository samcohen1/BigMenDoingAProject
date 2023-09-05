#include "Player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

Player::Player() {
    this->init_player();
}

void Player::init_player() {

    if(!this->player_texture_.loadFromFile("resources/Player.PNG")) return;
    this->player_sprite_.setTexture(this->player_texture_);
    this->player_sprite_.setPosition(900.f,300.0f);
    this->player_sprite_.setScale(0.08f,0.08f);

}

void Player::horizontal_move(Direction direction) {
    
}

void Player::vertical_move(Direction direction) {
    
}

void Player::render(sf::RenderTarget &target) {
    target.draw(player_sprite_);
}
