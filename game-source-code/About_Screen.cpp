#include "About_Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

About_Screen::About_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font) : 
        window_(window), background_sprite_(background_sprite), pixel_font_(pixel_font) {
    this->_init_about_screen();
}

void About_Screen::run() {
     while(this->window_->isOpen()) {
        this->update();
        this->render();
    }
}

void About_Screen::_init_about_screen () {
    this->texts_ = std::vector<sf::Text>(3);
    this->texts_[0].setFont(this->pixel_font_);
    this->texts_[0].setFillColor(sf::Color::White);
    this->texts_[0].setString("About Graduation-Hat Hackers");
    this->texts_[0].setPosition(this->window_->getSize().x/2 - this->texts_[0].getGlobalBounds().width/2, 100.f);
    this->texts_[0].setCharacterSize(16.f);
    this->texts_[1].setFont(this->pixel_font_);
    this->texts_[1].setFillColor(sf::Color::White);
    this->texts_[1].setCharacterSize(16.f);
    std::string about_message = "For a Wits University student the campus is a daunting place.\n";
    about_message += "The professors constantly attack students with assignments\nand the scientists are dangerous to be around in general.\n";
    about_message +="And we have not even mentioned the happenings on East Campus!\n";
    about_message +="But one brave group have found a way to survive through\nthis tough existence: The software developers.\n";
    about_message +="Your job is to become a software developer.\n";
    about_message +="You must shoot code at these treacherous enemies\nand protect the future of the university,\nwhile simultaneously avoiding becoming caught up\n";
    about_message +="by the bombs and assignments of the evil faculties.";
    this->texts_[1].setString(about_message);
    if(!this->box_texture_.loadFromFile("resources/splash_background.png")) return;
    this->box_sprite_.setTexture(this->box_texture_);
    this->box_sprite_.setScale(1.3f, 0.7f);
    this->box_sprite_.setPosition(this->window_->getSize().x/2 - this->box_sprite_.getGlobalBounds().width/2, 80.f);
    this->texts_[1].setPosition(this->window_->getSize().x/2 - this->texts_[1].getGlobalBounds().width/2, 300.f);
    this->texts_[0].setPosition(this->window_->getSize().x/2 - this->texts_[0].getGlobalBounds().width/2, 150.f);
}

void About_Screen::update() {
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->window_->close();
        }
    }
}

void About_Screen::render() {
    this->window_->clear();
    this->window_->draw(this->background_sprite_);
    this->window_->draw(this->box_sprite_);
    for (auto text : this->texts_) this->window_->draw(text);
    this->window_->display();
}
