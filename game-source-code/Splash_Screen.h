/** \file Splash_Screen.h
    \brief Contains the definitions of the splash screen which serves as the main menu.
 */

#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Game.h"

enum class Option {
    PLAY = 0,
    ABOUT,
    HOW_TO_PLAY
};

class Splash_Screen {
    private:
        std::shared_ptr<sf::RenderWindow> window_;
        
        bool key_held_down_ = false;
        bool mouse_is_over_ = false;

        Option option_selected_ = Option::PLAY;

        sf::Texture background_texture_;
        sf::Texture options_texture_;
        sf::Sprite background_sprite_;
        std::vector<sf::Sprite> option_sprites_;

        std::vector<sf::Text> texts_;
        sf::Text heading_;
        sf::Font pixel_font_;

        const float original_background_width_ = 1920;
        const float original_background_height_ = 945;
        const float game_width_ = 1400;
        const float game_height_ = 800;
        const float x_scale_ = this->game_width_/this->original_background_width_;
        const float y_scale_ = this->game_height_/this->original_background_height_;


        /** \fn void Splash_Screen::update()
         *  \brief Update the splash screen.
         *  This function handles user input and transitions to other screens based on user selections.
         */
        void update();


        /** \fn void Splash_Screen::render()
         *  \brief Render the splash screen.
         *  This function clears the window, draws all the splash screen elements, and displays the window.
         */
        void render();

        /** \fn void Splash_Screen::_init_window()
         *  \brief Initialize the game window.
         *  This function creates and initializes the game window with a specified size and title.
         */
        void _init_window();

        /** \fn void Splash_Screen::_init_background()
         *  \brief Initialize the background of the splash screen.
         *  This function loads and sets up the background texture for the splash screen.
         */
        void _init_background();

        /** \fn void Splash_Screen::_init_splash_screen()
         *  \brief Initialize the splash screen elements.
         *  This function loads and sets up the splash screen's options, text, and fonts.
         */
        void _init_splash_screen();


        /** \fn void Splash_Screen::shift_option_down()
         *  \brief Shift the selected option down.
         *  This function changes the selected option to the one below it and updates the visuals accordingly.
         */
        void shift_option_down();


        /** \fn void Splash_Screen::shift_option_up()
         *  \brief Shift the selected option up.
         *  This function changes the selected option to the one above it and updates the visuals accordingly.
         */
        void shift_option_up();


        /** \fn void Splash_Screen::select_using_mouse()
         *  \brief Select an option using the mouse.
         *  This function checks if the mouse pointer is over an option and updates the selected option accordingly.
         */
        void select_using_mouse();

    public:

        /** \fn Splash_Screen::Splash_Screen()
         *  \brief Constructor for Splash_Screen.
         *  This constructor initializes a Splash_Screen object by setting up the game window, background, and splash screen elements.
         */
        Splash_Screen();

        /** \fn void Splash_Screen::run()
         *  \brief Run the splash screen.
         *  This function starts the main loop for the splash screen, handling user input and rendering.
         */
        void run();
};

#endif