#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

#include "Splash_Screen.h"
#include "About_Screen.h"
#include "How_To_Play_Screen.h"
#include "Game.h"

Splash_Screen::Splash_Screen() {
    this->_init_window();
    this->_init_background();
    this->_init_splash_screen();
}

void Splash_Screen::_init_background() {
    if(!(this->background_texture_.loadFromFile("resources/Background.png"))) return;
    this->background_sprite_.setTexture(this->background_texture_);
    this->background_sprite_.setPosition(-2.f*this->game_width_, 0.f);
    this->background_sprite_.setScale(this->x_scale_, this->y_scale_);
}

void Splash_Screen::_init_window() {
    this->window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(this->game_width_, this->game_height_, 32), "Graduation-Hat Hackers", sf::Style::Titlebar | sf::Style::Close);
}

void Splash_Screen::run() {
    while(this->window_->isOpen()) {
        this->update();
        this->render();
    }
}

void Splash_Screen::update() {
    sf::Event event;
    this->mouse_is_over_ = false;

    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->window_->close();
        }
    }
    if(static_cast<int>(this->option_selected_) < static_cast<int>(Option::HOW_TO_PLAY) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !key_held_down_) {
       this->shift_option_down();
    }
    if(static_cast<int>(this->option_selected_) > static_cast<int>(Option::PLAY) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !key_held_down_) {
       this->shift_option_up();
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
        this->key_held_down_=false;
    }
    this->select_using_mouse();
    if((mouse_is_over_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))  || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !this->key_held_down_)) {
        if(this->option_selected_ == Option::PLAY) {
            // GO INTO GAME
            auto game = Game(this->window_, original_background_width_, original_background_height_, game_width_, game_height_, x_scale_, y_scale_);
            game.run();
        }
        if(this->option_selected_ == Option::ABOUT) {
            // GO INTO ABOUT
            this->key_held_down_ = true;
            auto about_screen = About_Screen(this->window_, this->background_sprite_, this->pixel_font_);
            about_screen.run();
        }
        if(this->option_selected_ == Option::HOW_TO_PLAY) {
            this->key_held_down_ = true;
            auto how_to_play_screen = How_To_Play_Screen(this->window_, this->background_sprite_, this->pixel_font_);
            how_to_play_screen.run();
        }
    }
}

void Splash_Screen::render() { 
    this->window_->clear();
    this->window_->draw(this->background_sprite_);
    this->window_->draw(this->heading_);
    for (auto i = 0; i < this->option_sprites_.size(); i++) {
        this->window_->draw(this->option_sprites_[i]);
        this->window_->draw(this->texts_[i]);
    }
    this->window_->display();
}

void Splash_Screen::_init_splash_screen() {
    this->option_sprites_ = std::vector<sf::Sprite>(3);
    this->texts_ = std::vector<sf::Text>(3);
    if(!this->options_texture_.loadFromFile("resources/home_options_spritesheet.png")) return;
    if(!this->pixel_font_.loadFromFile("resources/pixel_text.ttf")) return;
    this->heading_.setFont(this->pixel_font_);
    this->heading_.setFillColor(sf::Color::Black);
    this->heading_.setCharacterSize(40.f);
    this->heading_.setString("Graduation-Hat Hackers");
    this->heading_.setPosition(this->window_->getSize().x/2 - this->heading_.getGlobalBounds().width/2, 80.f);
    this->heading_.setOutlineColor(sf::Color(0, 192, 248));
    this->heading_.setOutlineThickness(3.f);
    float x_position = (this->window_->getSize().x/2.f) - 120.f;
    this->texts_[0].setFont(this->pixel_font_);
    this->texts_[0].setFillColor(sf::Color::White);
    this->texts_[0].setCharacterSize(18.f);
    this->texts_[0].setString("Play");
    this->option_sprites_[0].setTexture(this->options_texture_);
    this->option_sprites_[0].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
    this->option_sprites_[0].setScale(0.5f, 0.5f);
    float y_position_text = this->option_sprites_[0].getGlobalBounds().height/2 - this->texts_[0].getGlobalBounds().height;
    float x_position_text = this->option_sprites_[0].getGlobalBounds().width/2 - this->texts_[0].getGlobalBounds().width/2;
    this->option_sprites_[0].setPosition(x_position, 220.f);
    this->texts_[0].setPosition(x_position + x_position_text, 225.f + y_position_text);
    this->option_sprites_[1].setTexture(this->options_texture_);
    this->option_sprites_[1].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    this->option_sprites_[1].setPosition(x_position, 370.f);
    this->option_sprites_[1].setScale(0.5f, 0.5f);
    this->texts_[1].setFont(this->pixel_font_);
    this->texts_[1].setFillColor(sf::Color::White);
    this->texts_[1].setString("Backstory");
    this->texts_[1].setCharacterSize(18.f);
    x_position_text = this->option_sprites_[1].getGlobalBounds().width/2 - this->texts_[1].getGlobalBounds().width/2;
    this->texts_[1].setPosition(x_position + x_position_text, 375.f + y_position_text);
    this->option_sprites_[2].setTexture(this->options_texture_);
    this->option_sprites_[2].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    this->option_sprites_[2].setPosition(x_position, 520.f);
    this->option_sprites_[2].setScale(0.5f, 0.5f);
    this->texts_[2].setFont(this->pixel_font_);
    this->texts_[2].setFillColor(sf::Color::White);
    this->texts_[2].setString("How to Play");
    this->texts_[2].setCharacterSize(18.f);
    x_position_text = this->option_sprites_[2].getGlobalBounds().width/2 - this->texts_[2].getGlobalBounds().width/2;
    this->texts_[2].setPosition(x_position + x_position_text, 525.f + y_position_text);
}

void Splash_Screen::shift_option_down() {
    this->key_held_down_ = true;
    this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    this->option_selected_ = static_cast<Option>(static_cast<int>(this->option_selected_)+1);
    this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
}

void Splash_Screen::shift_option_up() {
    this->key_held_down_ = true;
    this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    this->option_selected_ = static_cast<Option>(static_cast<int>(this->option_selected_)-1);
    this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
}

void Splash_Screen::select_using_mouse() {
    sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*this->window_).x, sf::Mouse::getPosition(*this->window_).y);
    if(this->option_sprites_[0].getGlobalBounds().contains(mouse_position)){
        this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
        this->option_selected_ = Option::PLAY;
        this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
        this->mouse_is_over_ = true;
    }
    else if(this->option_sprites_[1].getGlobalBounds().contains(mouse_position)){
        this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
        this->option_selected_ = Option::ABOUT;
        this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
        this->mouse_is_over_ = true;
    }
    else if(this->option_sprites_[2].getGlobalBounds().contains(mouse_position)){
        this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
        this->option_selected_ = Option::HOW_TO_PLAY;
        this->option_sprites_[static_cast<int>(this->option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
        this->mouse_is_over_ = true;
    }
}
