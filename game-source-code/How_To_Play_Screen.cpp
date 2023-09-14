/** \file How_To_Play.cpp
    \brief Contains the implementation of the how to play screen
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include "How_To_Play_Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

How_To_Play_Screen::How_To_Play_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font) : 
        window_(window), background_sprite_(background_sprite), pixel_font_(pixel_font) {
    _init_how_to_play_screen();
}

void How_To_Play_Screen::run() {
     while(window_->isOpen()) {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) key_held_down_ = false;
        if((is_back_button_hovered() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !key_held_down_)) return;
        update();
        render();
    }
}

void How_To_Play_Screen::_init_how_to_play_screen () {
    if(!back_button_texture_.loadFromFile("resources/back_button_spritesheet.png") || !arrow_keys_texture_.loadFromFile("resources/arrow_keys.png") || !space_bar_texture.loadFromFile("resources/space_bar.png")) return;
    back_button_sprite_.setTexture(back_button_texture_);
    back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 346.f, 361.f));
    back_button_sprite_.setScale(0.2f, 0.2f);

    texts_ = std::vector<sf::Text>(3);
    texts_[0].setFont(pixel_font_);
    texts_[0].setFillColor(sf::Color::White);
    texts_[0].setString("How to Play!");
    texts_[0].setPosition(window_->getSize().x/2 - texts_[0].getGlobalBounds().width/2, 100.f);
    texts_[0].setCharacterSize(24.f);
    texts_[1].setFont(pixel_font_);
    texts_[1].setFillColor(sf::Color::White);
    texts_[1].setCharacterSize(16.f);
    std::string instruction_message = "Your task is to control Software-Dev Dude as he flies around the\n\nscreen avoiding enemy fire and prtecting the engineering students!\n\n\n\n";
    instruction_message += "-> To move around use the arrow keys on your keyboard!\n\n\n\n\n\n\n\n\n\n";
    instruction_message +="-> To shoot the enemies use your space-bar!\n\n\n\n\n\n\n\n\n\n";
    instruction_message +="-> You can pause the game anytime by pressing 'P' on your keyboard!\n\n";
    texts_[1].setString(instruction_message);
    if(!box_texture_.loadFromFile("resources/splash_background.png")) return;
    box_sprite_.setTexture(box_texture_);
    box_sprite_.setScale(1.3f, 0.7f);
    box_sprite_.setPosition(window_->getSize().x/2 - box_sprite_.getGlobalBounds().width/2, 80.f);
    texts_[1].setPosition(window_->getSize().x/2 - texts_[1].getGlobalBounds().width/2, 250.f);
    texts_[0].setPosition(window_->getSize().x/2 - texts_[0].getGlobalBounds().width/2, 150.f);
    back_button_sprite_.setPosition(box_sprite_.getPosition().x + 50.f, box_sprite_.getPosition().y + 30.f);

    arrow_keys_sprite_.setTexture(arrow_keys_texture_);
    arrow_keys_sprite_.setScale(0.25f, 0.25f);
    arrow_keys_sprite_.setPosition(window_->getSize().x/2-arrow_keys_sprite_.getGlobalBounds().width/2, texts_[1].getGlobalBounds().top+125.f);

    space_bar_sprite_.setTexture(space_bar_texture);
    space_bar_sprite_.setScale(0.3f, 0.3f);
    space_bar_sprite_.setPosition(window_->getSize().x/2-space_bar_sprite_.getGlobalBounds().width/2, texts_[1].getGlobalBounds().top+315.f);

    texts_[2].setFont(pixel_font_);
    texts_[2].setFillColor(sf::Color(0, 192, 248));
    texts_[2].setString("SPACE");
    texts_[2].setPosition(window_->getSize().x/2 - texts_[2].getGlobalBounds().width/2 + 25, space_bar_sprite_.getGlobalBounds().top+13);
    texts_[2].setCharacterSize(18.f);
}

void How_To_Play_Screen::update() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            window_->close();
        }
    }
    
}

void How_To_Play_Screen::render() {
    window_->clear();
    window_->draw(background_sprite_);
    window_->draw(box_sprite_);
    window_->draw(back_button_sprite_);
    window_->draw(arrow_keys_sprite_);
    window_->draw(space_bar_sprite_);
    for (auto text : texts_) window_->draw(text);
    window_->display();
}

bool How_To_Play_Screen::is_back_button_hovered()
{
    sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);
    if(back_button_sprite_.getGlobalBounds().contains(mouse_position)){
        back_button_sprite_.setTextureRect(sf::IntRect(392.f, 0.f, 345.f, 361.f));
        return true;
    }
    back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 345.f, 361.f));
    return false;
}
