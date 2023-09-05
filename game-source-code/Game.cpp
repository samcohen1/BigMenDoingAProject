#include <SFML/Graphics.hpp>
#include <memory>

#include "Game.h"


Game::Game() {
    this->_init_window();
    this->_init_background();
    this->_init_player();
}

void Game::_init_window() {
    this->window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(this->game_width_, this->game_height_, 32), "Byte Defenders", sf::Style::Titlebar | sf::Style::Close);
}

void Game::_init_background() {
    if(!(this->background_texture_.loadFromFile("resources/Background.png"))) return;
    this->background_sprite_.setTexture(this->background_texture_);
    this->background_sprite_.setPosition(-2.f*this->game_width_, 0.f);
    this->background_sprite_.setScale(this->x_scale_, this->y_scale_);
}

void Game::_init_player() {
    this->player_ = std::make_unique<Player>();
}

void Game::update() {
    
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window_->close();
        }
    }
}

void Game::render() {
    this->window_->clear();
    this->window_->draw(this->background_sprite_);
    this->player_->render(*this->window_);
    this->window_->display();
}

void Game::run() {
    while(this->window_->isOpen()) {
        this->update();
        this->render();
    }
}
