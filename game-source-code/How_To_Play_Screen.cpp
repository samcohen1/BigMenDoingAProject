
///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include "How_To_Play_Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

/** 
 * \fn How_To_Play_Screen::How_To_Play_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font)
 * \brief Constructor for the How_To_Play_Screen class, initializes the screen with necessary parameters and calls initialization method
 *
 * \param window A shared pointer to the window object where the screen will be displayed
 * \param background_sprite The sprite used as the background for the screen
 * \param pixel_font The font used for text elements on the screen
 */
How_To_Play_Screen::How_To_Play_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font) : 
        window_(window), background_sprite_(background_sprite), pixel_font_(pixel_font) {
    this->_init_how_to_play_screen();
}

/** 
 * \fn void How_To_Play_Screen::run()
 * \brief Runs the main loop for the How_To_Play screen, checking for user inputs and updating the display
 *
 * \return void
 */
void How_To_Play_Screen::run() {
     while(this->window_->isOpen()) {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) this->key_held_down_ = false;
        if((this->is_back_button_hovered() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !this->key_held_down_)) return;
        this->update();
        this->render();
    }
}

/** 
 * \fn void How_To_Play_Screen::_init_how_to_play_screen()
 * \brief Initializes elements of the How_To_Play screen including textures, sprites and text elements
 * 
 * \return void
 */
void How_To_Play_Screen::_init_how_to_play_screen () {
    if(!this->back_button_texture_.loadFromFile("resources/back_button_spritesheet.png") || !this->arrow_keys_texture_.loadFromFile("resources/arrow_keys.png") || !this->space_bar_texture.loadFromFile("resources/space_bar.png")) return;
    this->back_button_sprite_.setTexture(this->back_button_texture_);
    this->back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 346.f, 361.f));
    this->back_button_sprite_.setScale(0.2f, 0.2f);

    this->texts_ = std::vector<sf::Text>(3);
    this->texts_[0].setFont(this->pixel_font_);
    this->texts_[0].setFillColor(sf::Color::White);
    this->texts_[0].setString("How to Play!");
    this->texts_[0].setPosition(this->window_->getSize().x/2 - this->texts_[0].getGlobalBounds().width/2, 100.f);
    this->texts_[0].setCharacterSize(24.f);
    this->texts_[1].setFont(this->pixel_font_);
    this->texts_[1].setFillColor(sf::Color::White);
    this->texts_[1].setCharacterSize(16.f);
    std::string instruction_message = "Your task is to control Software-Dev Dude as he flies around the\n\nscreen avoiding enemy fire and prtecting the engineering students!\n\n\n\n";
    instruction_message += "-> To move around use the arrow keys on your keyboard!\n\n\n\n\n\n\n\n\n\n";
    instruction_message +="-> To shoot the enemies use your space-bar!\n\n\n\n\n\n\n\n\n\n";
    instruction_message +="-> You can pause the game anytime by pressing 'P' on your keyboard!\n\n";
    this->texts_[1].setString(instruction_message);
    if(!this->box_texture_.loadFromFile("resources/splash_background.png")) return;
    this->box_sprite_.setTexture(this->box_texture_);
    this->box_sprite_.setScale(1.3f, 0.7f);
    this->box_sprite_.setPosition(this->window_->getSize().x/2 - this->box_sprite_.getGlobalBounds().width/2, 80.f);
    this->texts_[1].setPosition(this->window_->getSize().x/2 - this->texts_[1].getGlobalBounds().width/2, 250.f);
    this->texts_[0].setPosition(this->window_->getSize().x/2 - this->texts_[0].getGlobalBounds().width/2, 150.f);
    this->back_button_sprite_.setPosition(this->box_sprite_.getPosition().x + 50.f, this->box_sprite_.getPosition().y + 30.f);

    this->arrow_keys_sprite_.setTexture(this->arrow_keys_texture_);
    this->arrow_keys_sprite_.setScale(0.25f, 0.25f);
    this->arrow_keys_sprite_.setPosition(this->window_->getSize().x/2-this->arrow_keys_sprite_.getGlobalBounds().width/2, this->texts_[1].getGlobalBounds().top+125.f);

    this->space_bar_sprite_.setTexture(this->space_bar_texture);
    this->space_bar_sprite_.setScale(0.3f, 0.3f);
    this->space_bar_sprite_.setPosition(this->window_->getSize().x/2-this->space_bar_sprite_.getGlobalBounds().width/2, this->texts_[1].getGlobalBounds().top+315.f);

    this->texts_[2].setFont(this->pixel_font_);
    this->texts_[2].setFillColor(sf::Color(0, 192, 248));
    this->texts_[2].setString("SPACE");
    this->texts_[2].setPosition(this->window_->getSize().x/2 - this->texts_[2].getGlobalBounds().width/2 + 25, this->space_bar_sprite_.getGlobalBounds().top+13);
    this->texts_[2].setCharacterSize(18.f);
}

/** 
 * \fn void How_To_Play_Screen::update()
 * \brief Handles events during the runtime of the How_To_Play screen, including window close and escape key press events
 * 
 * \return void
 */
void How_To_Play_Screen::update() {
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->window_->close();
        }
    }
    
}

/** 
 * \fn void How_To_Play_Screen::render()
 * \brief Renders all the elements of the How_To_Play screen to the window
 * 
 * \return void
 */
void How_To_Play_Screen::render() {
    this->window_->clear();
    this->window_->draw(this->background_sprite_);
    this->window_->draw(this->box_sprite_);
    this->window_->draw(this->back_button_sprite_);
    this->window_->draw(this->arrow_keys_sprite_);
    this->window_->draw(this->space_bar_sprite_);
    for (auto text : this->texts_) this->window_->draw(text);
    this->window_->display();
}

/** 
 * \fn bool How_To_Play_Screen::is_back_button_hovered()
 * \brief Checks if the back button is hovered over by the mouse and updates the button's texture accordingly
 *
 * \return true If the back button is hovered over
 * \return false Otherwise
 */
bool How_To_Play_Screen::is_back_button_hovered()
{
    sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*this->window_).x, sf::Mouse::getPosition(*this->window_).y);
    if(this->back_button_sprite_.getGlobalBounds().contains(mouse_position)){
        this->back_button_sprite_.setTextureRect(sf::IntRect(392.f, 0.f, 345.f, 361.f));
        return true;
    }
    this->back_button_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 345.f, 361.f));
    return false;
}
