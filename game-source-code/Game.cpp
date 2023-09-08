#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>

#include "Game.h"
#include "Bullet.h"
#include "Professor.h"
#include "Player.h"
#include "Professor_Assignment.h"

Game::Game() {
    srand(time(0));
    this->_init_window();
    this->_init_background();
    this->_init_textures();
    this->_init_player();
    this->_init_professor();
}

void Game::update() {
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->window_->close();
        }
    }
    this->handle_player_movement();
    this->check_player_shoot();
    this->check_professors_shoot();
    for (auto i = 0; i < professors_.size(); i++) {
        this->professors_[i]->move_professor(this->background_location_, sf::Vector2f(this->player_->get_position().x_left, this->player_->get_position().y));
    }
    this->teleport_professor();
    // if (Professor::get_num_professors() < 5) {
    //     if(this->professor_cool_down > this->max_professor_cool_down) {
    //         this->_init_professor();
    //         this->professor_cool_down = 0;
    //     }
    //     else {
    //         this->professor_cool_down++;
    //     }
    // }
}

void Game::render() {
    this->window_->clear(sf::Color(110,66,26));
    this->window_->draw(this->background_sprite_);
    this->player_->render(*this->window_, this->background_movement_);
    for (auto i = 0; i < professors_.size(); i++) {
        this->professors_[i]->render(*this->window_, this->background_movement_tracker);
    }
    this->window_->display();
}

void Game::run() {
    while(this->window_->isOpen()) {
        this->update();
        this->render();
    }
}

void Game::_init_window() {
    this->window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(this->game_width_, this->game_height_, 32), "Byte Defenders", sf::Style::Titlebar | sf::Style::Close);
}

void Game::_init_textures () {
    // Initialise Player Bullet Texture
    sf::Texture bullet_texture;
    if(!bullet_texture.loadFromFile("resources/Bullet.png")) return;
    this->textures.push_back(bullet_texture);
    // Initialise Player-sheet Texture
    sf::Texture player_sheet_texture;
    if(!player_sheet_texture.loadFromFile("resources/Player_Spritesheet.png")) return;
    this->textures.push_back(player_sheet_texture);
    // Initialise Professor-sheet Texture
    sf::Texture professor_sheet_texture;
    if(!professor_sheet_texture.loadFromFile("resources/Evil_Professor_Spritesheet.png")) return;
    this->textures.push_back(professor_sheet_texture);
    // Initialise Professor-Assignment Texture
    sf::Texture professor_assignment_texture;
    if(!professor_assignment_texture.loadFromFile("resources/ProfessorBullet.png")) return;
    this->textures.push_back(professor_assignment_texture);
}

void Game::_init_background() {
    if(!(this->background_texture_.loadFromFile("resources/Background.png"))) return;
    this->background_sprite_.setTexture(this->background_texture_);
    this->background_sprite_.setPosition(-2.f*this->game_width_, 0.f);
    this->background_sprite_.setScale(this->x_scale_, this->y_scale_);
}

void Game::_init_player() {
    this->player_ = std::make_unique<Player>(this->textures[static_cast<int>(Textures::PLAYER_SHEET)]);
}

void Game::_init_professor() {
    this->professors_.push_back(std::make_unique<Professor>(this->textures[static_cast<int>(Textures::PROFESSOR_SHEET)]));
}

void Game::teleport_professor () {
    if (Professor::get_num_professors() < 5) {
        if(this->professor_cool_down > this->max_professor_cool_down) {
            this->_init_professor();
            this->professor_cool_down = 0;
        }
        else {
            this->professor_cool_down++;
        }
    }
}

bool Game::approx_equal (float a, float b) {
    float tolerance = 0.01f;
    return abs(a-b) <= tolerance;
}

bool Game::approx_innequality (float a, float b, bool greater_than) {
    float tolerance = 0.01;
    if (greater_than) return a > b+tolerance;
    return a < b+tolerance;
}

void Game::check_player_shoot() {
    this->player_->increment_cool_down();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !this->shot_held) {
        this->player_->shoot_bullet(this->textures[static_cast<int>(Textures::BULLET)]);
        this->shot_held = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        this->shot_held = false;
    }
}

void Game::check_professors_shoot() {
    for (auto i = 0; i < this->professors_.size(); i++){
        this->professors_[i]->increment_cool_down();
        this->professors_[i]->shoot_assignment(this->textures[static_cast<int>(Textures::PROFESSOR_ASSIGNMENT)], sf::Vector2f(this->player_->get_position().x_left, this->player_->get_position().y));
    }
}

void Game::handle_boundary_background_movement () {
    float tolerance = 0.0001f;
    if (this->background_location_<2*this->game_width_ && this->background_location_>-2*this->game_width_) {
        if (this->background_movement_ > tolerance) {
            this->background_movement_ -= background_acceleration_;
            this->background_movement_tracker = this->background_movement_;
            this->background_sprite_.move(this->background_movement_,0.f);
        }
        else if (this->background_movement_ < -tolerance) {
            this->background_movement_ += background_acceleration_;
            this->background_movement_tracker = this->background_movement_;
            this->background_sprite_.move(this->background_movement_,0.f); 
        } else {
            this->background_movement_ = 0;
            this->background_movement_tracker = this->background_movement_;
        }
    } else {
        this->background_movement_ = 0;
        this->background_movement_tracker = this->background_movement_;
    }
}

void Game::handle_internal_background_movement () {
    float player_internal_movement = this->player_->get_player_speed();
    this->background_movement_tracker = -player_internal_movement;
    this->background_sprite_.move(-player_internal_movement, 0.f);
    this->background_location_ += -player_internal_movement;
}

void Game::internal_movement (float x_right, float x_left) {
    if (this->prev_in_edge) this->player_->correct_edge_positions();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        if (x_left > this->player_->get_x_default_left()) {
            this->player_->move_player_horizontal(Direction::LEFT);
            this->handle_internal_background_movement();
            this->background_movement_ = 0.f;
        }
        else if (this->background_location_ > -2*this->game_width_) {
            this->background_sprite_.move(-this->background_base_speed_, 0.f);
            this->background_movement_ = -this->background_base_speed_;
            this->background_movement_tracker = this->background_movement_;
        } else {
            this->background_movement_ = 0;
            this->background_movement_tracker = this->background_movement_;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        if(x_right < this->player_->get_x_default_right()) {
            this->player_->move_player_horizontal(Direction::RIGHT);
            this->handle_internal_background_movement();
            this->background_movement_ = 0.f;
        }
        else if (this->background_location_ < 2*this->game_width_)  {
            this->background_sprite_.move(this->background_base_speed_, 0.f);
            this->background_movement_ = this->background_base_speed_;
            this->background_movement_tracker = this->background_movement_;
        } else {
            this->background_movement_ = 0;
            this->background_movement_tracker = this->background_movement_;
        }
    }

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
        this->player_->magnetise_player();
        if ((x_right < this->player_->get_x_default_right() && this->player_->get_player_speed() < 0) || 
            (x_left > this->player_->get_x_default_left() && this->player_->get_player_speed() > 0)) {
                this->handle_internal_background_movement();
            } else this->handle_boundary_background_movement();
    }
    this->background_location_ += this->background_movement_;
    this->prev_in_edge = false;
}

void Game::edge_movement(float x_right, float x_left) {
    bool sideKey = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && this->player_->get_position().x_right <= 1377.f) {
        sideKey = true;
        this->player_->edge_player_movement(Direction::LEFT);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && this->player_->get_position().x_left >= 23.f) {
        sideKey = true;
        this->player_->edge_player_movement(Direction::RIGHT);
    }
    if(!sideKey && this->player_->get_position().x_left >= 23.f && this->player_->get_position().x_right <= 1377.f) {
        this->player_->edge_decelerate();
    }
    this->prev_in_edge = true;
}

void Game::handle_player_movement() {
    float x_right = this->player_->get_position().x_right;
    float x_left = this->player_->get_position().x_left;
    float edge_tolerance = 2.f;
    bool in_edge_case = (this->background_location_ > 2*this->game_width_-edge_tolerance && x_left <= this->player_->get_x_default_right()+edge_tolerance) || 
                        (this->background_location_ < -2*this->game_width_ && x_right >= this->player_->get_x_default_left()-edge_tolerance);

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        if (this->player_->get_prev_vertical_direction() == Direction::UP) this->player_->move_player_vertical(Direction::UP, false);
        if (this->player_->get_prev_vertical_direction() == Direction::DOWN) this->player_->move_player_vertical(Direction::DOWN, false);
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player_vertical(Direction::UP, true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player_vertical(Direction::DOWN, true);
    }

    if (in_edge_case) {
        this->background_movement_tracker = 0.f;
        this->edge_movement(x_right, x_left);
        return;
    } else this->internal_movement(x_right, x_left);
}
