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

    bool horizontal_movement_ = false;
    float x_right = this->player_->get_position().x_right;
    float x_left = this->player_->get_position().x_left;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        horizontal_movement_ = true;
        if(x_left > this->player_->get_x_default_left()) {
            this->player_->move_player(Direction::LEFT);
            this->background_movement_ = 0.f;
        }
        else {
            if(this->background_location_>-2*this->game_width_) {
                this->background_sprite_.move(-0.3f, 0.f);
                this->background_movement_ = -0.3f;
            }
            else{
                this->background_movement_ = 0.f;
            }
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        horizontal_movement_ = true;
        if(x_right < this->player_->get_x_default_right()) {
            this->player_->move_player(Direction::RIGHT);
            this->background_movement_ = 0.f;
        }
        else {
            
            if(this->background_location_<2*this->game_width_) {
                this->background_sprite_.move(0.3f, 0.f);
                this->background_movement_ = 0.3f;
            } else {
                this->background_movement_ = 0.f;
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player(Direction::UP);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player(Direction::DOWN);

    if(!horizontal_movement_ && this->background_location_<2*this->game_width_ && this->background_location_>-2*this->game_width_) {
        if(this->background_movement_ > 0.0001) {
            this->background_movement_ -= 0.0001;
            this->background_sprite_.move(this->background_movement_,0.f);
            
            
        }
        else if(this->background_movement_ < -0.0001) {
            this->background_movement_ += 0.0001;
            this->background_sprite_.move(this->background_movement_,0.f);
            
        }
        else{
             this->background_movement_ = 0;
        }
    }
    this->background_location_ += this->background_movement_;
    std::cout << this->background_location_<<std::endl;
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
