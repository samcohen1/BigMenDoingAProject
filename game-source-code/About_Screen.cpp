
///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include "About_Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

/**
 * \fn About_Screen::About_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font)
 * \brief This constructor initializes the About_Screen class with the specified window, background sprite and pixel font.
 *        It also calls the _init_about_screen method to initialize the about screen.
 *
 * \param window The render window to which the About_Screen is bound.
 * \param background_sprite The background sprite used in the about screen.
 * \param pixel_font The font used for the text in the about screen.
 */
About_Screen::About_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font) : 
        window_(window), background_sprite_(background_sprite), pixel_font_(pixel_font) {
    this->_init_about_screen();
}

/**
 * \fn void About_Screen::run()
 * \brief This function contains the main loop for the about screen, handling input and invoking update and render methods.
 *
 * \return Returns nothing but exits when the back button is hovered and clicked or the enter key is pressed.
 */
void About_Screen::run() {
     while(this->window_->isOpen()) {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) this->key_held_down_ = false;
        if((this->is_back_button_hovered() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !this->key_held_down_)) return;
        this->update();
        this->render();


    }
}

/**
 * \fn void About_Screen::_init_about_screen()
 * \brief This method initializes the various attributes for the about screen including back button, texts, and other graphical elements.
 *
 * \return Returns nothing but initializes various attributes of the About_Screen class.
 */
void About_Screen::_init_about_screen () {
    if(!this->back_button_texture_.loadFromFile("resources/back_button_spritesheet.png")) return;
    this->back_button_sprite_.setTexture(this->back_button_texture_);
    this->back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 346.f, 361.f));
    this->back_button_sprite_.setScale(0.2f, 0.2f);
    this->texts_ = std::vector<sf::Text>(3);
    this->texts_[0].setFont(this->pixel_font_);
    this->texts_[0].setFillColor(sf::Color::White);
    this->texts_[0].setString("About Graduation-Hat Hackers");
    this->texts_[0].setPosition(this->window_->getSize().x/2 - this->texts_[0].getGlobalBounds().width/2, 100.f);
    this->texts_[0].setCharacterSize(24.f);
    this->texts_[1].setFont(this->pixel_font_);
    this->texts_[1].setFillColor(sf::Color::White);
    this->texts_[1].setCharacterSize(16.f);
    std::string about_message = "For a Wits University student the campus is a daunting place.\n\n";
    about_message += "The professors constantly attack students with assignments\n\nand the scientists are dangerous to be around in general.\n\n";
    about_message +="And we have not even mentioned the happenings on East Campus!\n\n";
    about_message +="But one brave group have found a way to survive through\n\nthis tough existence: The software developers.\n\n";
    about_message +="Your job is to become a software developer.\n\n";
    about_message +="You must shoot code at these treacherous enemies\n\nand protect the future of the university,\n\nwhile simultaneously avoiding becoming caught up\n\n";
    about_message +="by the bombs and assignments of the evil faculties.";
    this->texts_[1].setString(about_message);
    if(!this->box_texture_.loadFromFile("resources/splash_background.png")) return;
    this->box_sprite_.setTexture(this->box_texture_);
    this->box_sprite_.setScale(1.3f, 0.7f);
    this->box_sprite_.setPosition(this->window_->getSize().x/2 - this->box_sprite_.getGlobalBounds().width/2, 80.f);
    this->texts_[1].setPosition(this->window_->getSize().x/2 - this->texts_[1].getGlobalBounds().width/2, 290.f);
    this->texts_[0].setPosition(this->window_->getSize().x/2 - this->texts_[0].getGlobalBounds().width/2, 150.f);
    this->back_button_sprite_.setPosition(this->box_sprite_.getPosition().x + 50.f, this->box_sprite_.getPosition().y + 30.f);
}

/**
 * \fn void About_Screen::update()
 * \brief This method handles event polling including window close and escape key press events.
 *
 * \return Returns nothing but processes events during each iteration of the main loop.
 */
void About_Screen::update() {
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->window_->close();
        }
    }
    
}

/**
 * \fn void About_Screen::render()
 * \brief This method handles the rendering of all graphical elements onto the window.
 *
 * \return Returns nothing but draws various elements onto the window for each iteration of the main loop.
 */
void About_Screen::render() {
    this->window_->clear();
    this->window_->draw(this->background_sprite_);
    this->window_->draw(this->box_sprite_);
    for (auto text : this->texts_) this->window_->draw(text);
    this->window_->draw(this->back_button_sprite_);
    this->window_->display();
}

/**
 * \fn bool About_Screen::is_back_button_hovered()
 * \brief This method checks whether the back button is hovered by comparing the mouse position with the button's bounds.
 *
 * \return Returns true if the back button is hovered, otherwise returns false.
 */
bool About_Screen::is_back_button_hovered()
{
    sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*this->window_).x, sf::Mouse::getPosition(*this->window_).y);
    if(this->back_button_sprite_.getGlobalBounds().contains(mouse_position)){
        this->back_button_sprite_.setTextureRect(sf::IntRect(392.f, 0.f, 345.f, 361.f));
        return true;
    }
    this->back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 345.f, 361.f));
    return false;
}
