#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
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
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->window_->close();
        }
    }
    this->handle_player_movement();
}

void Game::handle_player_movement() {
    float x_right = this->player_->get_position().x_right;
    float x_left = this->player_->get_position().x_left;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        if(x_left > this->player_->get_x_default_left()) {
            this->player_->move_player(Direction::LEFT);
        }
        else {
            if(this->background_location_>-2*this->game_width_) {
                this->background_sprite_.move(-0.2f, 0.f);
                this->background_location_-=0.2f;
            }
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
         if(x_right < this->player_->get_x_default_right()) {
            this->player_->move_player(Direction::RIGHT);
        }
        else {
            
            if(this->background_location_<2*this->game_width_) {
                this->background_sprite_.move(0.2f, 0.f);
                this->background_location_ += 0.2f;
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player(Direction::UP);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player(Direction::DOWN);
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
