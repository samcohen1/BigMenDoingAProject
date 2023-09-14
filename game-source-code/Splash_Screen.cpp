/** \file Splash_Screen.cpp
    \brief Contains the implementation of the splash screen which serves as the main menu.
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

#include "Splash_Screen.h"
#include "About_Screen.h"
#include "How_To_Play_Screen.h"
#include "Game.h"


Splash_Screen::Splash_Screen() {

    //Initialise the various components required for the program and splash screen
    _init_window();
    _init_background();
    _init_splash_screen();
}


void Splash_Screen::_init_background() {

    //Load the background texture and set the sprite's position and scale
    if(!(background_texture_.loadFromFile("resources/background_all.png"))) return;
    background_sprite_.setTexture(background_texture_);
    background_sprite_.setPosition(-2.f*game_width_, 0.f);
    background_sprite_.setScale(x_scale_, y_scale_);
}


void Splash_Screen::_init_window() {

    //Create the window in which the game will run
    window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(game_width_, game_height_, 32), "Graduation-Hat Hackers", sf::Style::Titlebar | sf::Style::Close);
}


void Splash_Screen::run() {

    //Run while the window is open
    while(window_->isOpen()) {
        update();
        render();
    }
}


void Splash_Screen::update() {

    sf::Event event;
    mouse_is_over_ = false;

    //Check if the window is closed by pressing the escape key or close button
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            window_->close();
        }
    }

    //Shift option selected using the mouse or up and down arrow keys
    if(static_cast<int>(option_selected_) < static_cast<int>(Option::HOW_TO_PLAY) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !key_held_down_) {
       shift_option_down();
    }
    if(static_cast<int>(option_selected_) > static_cast<int>(Option::PLAY) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !key_held_down_) {
       shift_option_up();
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
        key_held_down_=false;
    }
    select_using_mouse();

    //Go into the chosen option if it is clicked on or the enter key is pressed
    if((mouse_is_over_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))  || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !key_held_down_)) {
        if(option_selected_ == Option::PLAY) {
            // GO INTO GAME
            auto game = Game(window_, original_background_width_, original_background_height_, game_width_, game_height_, x_scale_, y_scale_);
            game.run();
        }
        if(option_selected_ == Option::ABOUT) {
            // GO INTO ABOUT
            key_held_down_ = true;
            auto about_screen = About_Screen(window_, background_sprite_, pixel_font_);
            about_screen.run();
        }
        if(option_selected_ == Option::HOW_TO_PLAY) {
            key_held_down_ = true;
            auto how_to_play_screen = How_To_Play_Screen(window_, background_sprite_, pixel_font_);
            how_to_play_screen.run();
        }
    }
}


void Splash_Screen::render() { 

    //Render the objects on the screen
    window_->clear();
    window_->draw(background_sprite_);
    window_->draw(heading_);
    for (auto i = 0; i < option_sprites_.size(); i++) {
        window_->draw(option_sprites_[i]);
        window_->draw(texts_[i]);
    }
    window_->display();
}


void Splash_Screen::_init_splash_screen() {

    //Initialise the splash screen elements, the text and boxes
    option_sprites_ = std::vector<sf::Sprite>(3);
    texts_ = std::vector<sf::Text>(3);
    if(!options_texture_.loadFromFile("resources/home_options_spritesheet.png")) return;
    if(!pixel_font_.loadFromFile("resources/pixel_text.ttf")) return;
    heading_.setFont(pixel_font_);
    heading_.setFillColor(sf::Color::Black);
    heading_.setCharacterSize(40.f);
    heading_.setString("Graduation-Hat Hackers");
    heading_.setPosition(window_->getSize().x/2 - heading_.getGlobalBounds().width/2, 80.f);
    heading_.setOutlineColor(sf::Color(0, 192, 248));
    heading_.setOutlineThickness(3.f);
    float x_position = (window_->getSize().x/2.f) - 120.f;
    texts_[0].setFont(pixel_font_);
    texts_[0].setFillColor(sf::Color::White);
    texts_[0].setCharacterSize(18.f);
    texts_[0].setString("Play");
    option_sprites_[0].setTexture(options_texture_);
    option_sprites_[0].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
    option_sprites_[0].setScale(0.5f, 0.5f);
    float y_position_text = option_sprites_[0].getGlobalBounds().height/2 - texts_[0].getGlobalBounds().height;
    float x_position_text = option_sprites_[0].getGlobalBounds().width/2 - texts_[0].getGlobalBounds().width/2;
    option_sprites_[0].setPosition(x_position, 220.f);
    texts_[0].setPosition(x_position + x_position_text, 225.f + y_position_text);
    option_sprites_[1].setTexture(options_texture_);
    option_sprites_[1].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    option_sprites_[1].setPosition(x_position, 370.f);
    option_sprites_[1].setScale(0.5f, 0.5f);
    texts_[1].setFont(pixel_font_);
    texts_[1].setFillColor(sf::Color::White);
    texts_[1].setString("Backstory");
    texts_[1].setCharacterSize(18.f);
    x_position_text = option_sprites_[1].getGlobalBounds().width/2 - texts_[1].getGlobalBounds().width/2;
    texts_[1].setPosition(x_position + x_position_text, 375.f + y_position_text);
    option_sprites_[2].setTexture(options_texture_);
    option_sprites_[2].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    option_sprites_[2].setPosition(x_position, 520.f);
    option_sprites_[2].setScale(0.5f, 0.5f);
    texts_[2].setFont(pixel_font_);
    texts_[2].setFillColor(sf::Color::White);
    texts_[2].setString("How to Play");
    texts_[2].setCharacterSize(18.f);
    x_position_text = option_sprites_[2].getGlobalBounds().width/2 - texts_[2].getGlobalBounds().width/2;
    texts_[2].setPosition(x_position + x_position_text, 525.f + y_position_text);
}


void Splash_Screen::shift_option_down() {
    //Shift the option down if it is able to and change the colours of boxes
    key_held_down_ = true;
    option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    option_selected_ = static_cast<Option>(static_cast<int>(option_selected_)+1);
    option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
}


void Splash_Screen::shift_option_up() {
    //Shift the option up if it is able to and change the colours of boxes
    key_held_down_ = true;
    option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
    option_selected_ = static_cast<Option>(static_cast<int>(option_selected_)-1);
    option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
}


void Splash_Screen::select_using_mouse() {
    //Select the option using the mouse position and change the colours of boxes
    sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);
    if(option_sprites_[0].getGlobalBounds().contains(mouse_position)){
        option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
        option_selected_ = Option::PLAY;
        option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
        mouse_is_over_ = true;
    }
    else if(option_sprites_[1].getGlobalBounds().contains(mouse_position)){
        option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
        option_selected_ = Option::ABOUT;
        option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
        mouse_is_over_ = true;
    }
    else if(option_sprites_[2].getGlobalBounds().contains(mouse_position)){
        option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
        option_selected_ = Option::HOW_TO_PLAY;
        option_sprites_[static_cast<int>(option_selected_)].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
        mouse_is_over_ = true;
    }
}
