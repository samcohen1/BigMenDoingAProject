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

// void Game::handle_player_movement() {
//     float x_right = this->player_->get_position().x_right;
//     float x_left = this->player_->get_position().x_left;

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player_verticle(Direction::UP);
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player_verticle(Direction::DOWN);

//     if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
//         this->player_->magnatise_player();
//         return;
//     }

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
//         if(x_left > this->player_->get_x_default_left()) {
//             this->player_->move_player_horizontal(Direction::LEFT);
//         }
//         else {
//             this->background_sprite_.move(-0.3f, 0.f);
//         }
//     }
    
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
//         if(x_right < this->player_->get_x_default_right()) {
//             this->player_->move_player_horizontal(Direction::RIGHT);
//         }
//         else {
//             this->background_sprite_.move(0.3f, 0.f);
//         }
//     }
// }

bool Game::approx_equal (float a, float b) {
    float tolerance = 0.6f;
    return abs(a-b) <= tolerance;
}

void Game::handle_background_movement () {
    if (approx_equal(this->player_->get_position().x_left, this->player_->get_x_default_left()) || approx_equal(this->player_->get_position().x_right, this->player_->get_x_default_right())) {
        if (-this->background_speed_ < 0 && this->player_->get_prev_direction() == Direction::LEFT) {
            this->background_speed_ += -static_cast<float>(this->player_->get_prev_direction())*this->background_acceleration_;
            this->background_sprite_.move(-static_cast<float>(this->player_->get_prev_direction())*this->background_speed_, 0.f);
        }
        if (this->background_speed_ > 0 && (this->player_->get_prev_direction() == Direction::RIGHT)) {
            this->background_speed_ += static_cast<float>(this->player_->get_prev_direction())*this->background_acceleration_;
            this->background_sprite_.move(-static_cast<float>(this->player_->get_prev_direction())*this->background_speed_, 0.f);
        }
   }
}

void Game::handle_player_movement() {
    float x_right = this->player_->get_position().x_right;
    float x_left = this->player_->get_position().x_left;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player_verticle(Direction::UP);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player_verticle(Direction::DOWN);

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
        this->player_->magnatise_player();
        this->handle_background_movement();
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        if(x_left > this->player_->get_x_default_left()) {
            this->player_->move_player_horizontal(Direction::LEFT);
        }
        else {
            std::cout << "location = " << this->background_location_ << std::endl;
            this->background_speed_ = this->background_base_speed_;
            this->background_sprite_.move(-this->background_base_speed_, 0.f);
            this->background_location_ += -this->background_base_speed_;
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        if(x_right < this->player_->get_x_default_right()) {
            this->player_->move_player_horizontal(Direction::RIGHT);
        }
        else {
            this->background_speed_ = this->background_base_speed_;
            this->background_sprite_.move(this->background_base_speed_, 0.f);
            this->background_location_ += this->background_base_speed_;
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
