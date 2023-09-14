/** \file About_Screen.cpp
    \brief Contains the implementation of the about screen
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include "About_Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

About_Screen::About_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font) : 
        window_(window), background_sprite_(background_sprite), pixel_font_(pixel_font) {
    _init_about_screen();
}

void About_Screen::run() {
     while(window_->isOpen()) {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) key_held_down_ = false;
        if((is_back_button_hovered() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !key_held_down_)) return;
        update();
        render();


    }
}

void About_Screen::_init_about_screen () {
    if(!back_button_texture_.loadFromFile("resources/back_button_spritesheet.png")) return;
    back_button_sprite_.setTexture(back_button_texture_);
    back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 346.f, 361.f));
    back_button_sprite_.setScale(0.2f, 0.2f);
    texts_ = std::vector<sf::Text>(3);
    texts_[0].setFont(pixel_font_);
    texts_[0].setFillColor(sf::Color::White);
    texts_[0].setString("About Graduation-Hat Hackers");
    texts_[0].setPosition(window_->getSize().x/2 - texts_[0].getGlobalBounds().width/2, 100.f);
    texts_[0].setCharacterSize(24.f);
    texts_[1].setFont(pixel_font_);
    texts_[1].setFillColor(sf::Color::White);
    texts_[1].setCharacterSize(16.f);
    std::string about_message = "For a Wits University student the campus is a daunting place.\n\n";
    about_message += "The professors constantly attack students with assignments\n\nand the scientists are dangerous to be around in general.\n\n";
    about_message +="And we have not even mentioned the happenings on East Campus!\n\n";
    about_message +="But one brave group have found a way to survive through\n\nthis tough existence: The software developers.\n\n";
    about_message +="Your job is to become a software developer.\n\n";
    about_message +="You must shoot code at these treacherous enemies\n\nand protect the future of the university,\n\nwhile simultaneously avoiding becoming caught up\n\n";
    about_message +="by the bombs and assignments of the evil faculties.";
    texts_[1].setString(about_message);
    if(!box_texture_.loadFromFile("resources/splash_background.png")) return;
    box_sprite_.setTexture(box_texture_);
    box_sprite_.setScale(1.3f, 0.7f);
    box_sprite_.setPosition(window_->getSize().x/2 - box_sprite_.getGlobalBounds().width/2, 80.f);
    texts_[1].setPosition(window_->getSize().x/2 - texts_[1].getGlobalBounds().width/2, 290.f);
    texts_[0].setPosition(window_->getSize().x/2 - texts_[0].getGlobalBounds().width/2, 150.f);
    back_button_sprite_.setPosition(box_sprite_.getPosition().x + 50.f, box_sprite_.getPosition().y + 30.f);
}

void About_Screen::update() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            window_->close();
        }
    }
    
}

void About_Screen::render() {
    window_->clear();
    window_->draw(background_sprite_);
    window_->draw(box_sprite_);
    for (auto text : texts_) window_->draw(text);
    window_->draw(back_button_sprite_);
    window_->display();
}

bool About_Screen::is_back_button_hovered()
{
    sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);
    if(back_button_sprite_.getGlobalBounds().contains(mouse_position)){
        back_button_sprite_.setTextureRect(sf::IntRect(392.f, 0.f, 345.f, 361.f));
        return true;
    }
    back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 345.f, 361.f));
    return false;
}
