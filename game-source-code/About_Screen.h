/** \file About_Screen.h
    \brief Contains the definitions of the about screen
 */

#ifndef ABOUT_SCREEN_H
#define ABOUT_SCREEN_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>

class About_Screen {
    private:
    /************************************************************************************
    **                             INITIALISATION CONSTANTS                            **
    ************************************************************************************/
        // WINDOW
        std::shared_ptr<sf::RenderWindow> window_;
        bool key_held_down_ = true;

        //TEXTURES, SPRITES AND TEXTS
        sf::Texture box_texture_;
        sf::Texture back_button_texture_;

        sf::Sprite box_sprite_;
        sf::Sprite back_button_sprite_;
        sf::Sprite background_sprite_;
        
        sf::Font pixel_font_;
        std::vector<sf::Text> texts_;
        //***********************************************************************************

        /**
        * \fn void About_Screen::_init_about_screen()
        * \brief This method initializes the various attributes for the about screen including back button, texts, and other graphical elements.
        *
        * \return Returns nothing but initializes various attributes of the About_Screen class.
        */
        void _init_about_screen();

        /**
        * \fn void About_Screen::update()
        * \brief This method handles event polling including window close and escape key press events.
        *
        * \return Returns nothing but processes events during each iteration of the main loop.
        */
        void update();

        /**
        * \fn void About_Screen::render()
        * \brief This method handles the rendering of all graphical elements onto the window.
        *
        * \return Returns nothing but draws various elements onto the window for each iteration of the main loop.
        */
        void render();

        /**
        * \fn bool About_Screen::is_back_button_hovered()
        * \brief This method checks whether the back button is hovered by comparing the mouse position with the button's bounds.
        *
        * \return Returns true if the back button is hovered, otherwise returns false.
        */
        bool is_back_button_hovered();

    public:
        /**
        * \fn About_Screen::About_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font)
        * \brief This constructor initializes the About_Screen class with the specified window, background sprite and pixel font.
        *        It also calls the _init_about_screen method to initialize the about screen.
        *
        * \param window The render window to which the About_Screen is bound.
        * \param background_sprite The background sprite used in the about screen.
        * \param pixel_font The font used for the text in the about screen.
        */
        About_Screen(std::shared_ptr<sf::RenderWindow>, sf::Sprite, sf::Font);

        /**
        * \fn void About_Screen::run()
        * \brief This function contains the main loop for the about screen, handling input and invoking update and render methods.
        *
        * \return Returns nothing but exits when the back button is hovered and clicked or the enter key is pressed.
        */
        void run();
};


#endif