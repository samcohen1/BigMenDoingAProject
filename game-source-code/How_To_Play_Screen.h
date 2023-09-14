/** \file How_To_Play.h
    \brief Contains the definitions of the how to play screen
 */

#ifndef HOW_TO_PLAY_SCREEN_H
#define HOW_TO_PLAY_SCREEN_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class How_To_Play_Screen {
    private:
    /************************************************************************************
    **                             INITIALISATION CONSTANTS                            **
    ************************************************************************************/
        // WINDOW
        std::shared_ptr<sf::RenderWindow> window_;
        bool key_held_down_ = true;

        //TEXTURE, SPRITES, AND TEXTS
        sf::Texture box_texture_;
        sf::Texture back_button_texture_;
        sf::Texture arrow_keys_texture_;
        sf::Texture space_bar_texture;

        sf::Sprite box_sprite_;
        sf::Sprite background_sprite_;
        sf::Sprite back_button_sprite_;
        sf::Sprite arrow_keys_sprite_;
        sf::Sprite space_bar_sprite_;

        sf::Font pixel_font_;
        std::vector<sf::Text> texts_;
    //***********************************************************************************


        /** 
        * \fn void How_To_Play_Screen::_init_how_to_play_screen()
        * \brief Initializes elements of the How_To_Play screen including textures, sprites and text elements
        * 
        * \return void
        */
        void _init_how_to_play_screen();

        /** 
        * \fn void How_To_Play_Screen::update()
        * \brief Handles events during the runtime of the How_To_Play screen, including window close and escape key press events
        * 
        * \return void
        */
        void update();

        /** 
        * \fn void How_To_Play_Screen::render()
        * \brief Renders all the elements of the How_To_Play screen to the window
        * 
        * \return void
        */
        void render();

        /** 
        * \fn bool How_To_Play_Screen::is_back_button_hovered()
        * \brief Checks if the back button is hovered over by the mouse and updates the button's texture accordingly
        *
        * \return true If the back button is hovered over
        * \return false Otherwise
        */
        bool is_back_button_hovered();


        bool load_textures();
        void set_sprites();
        void set_sprite(sf::Sprite&, sf::Texture&, sf::IntRect, float, float);
        void set_texts();
        void set_text(sf::Text&, const std::string&, float, const sf::Color&, sf::Vector2f);
        std::string get_instruction_message();
    public:
        /** 
        * \fn How_To_Play_Screen::How_To_Play_Screen(std::shared_ptr<sf::RenderWindow> window, sf::Sprite background_sprite, sf::Font pixel_font)
        * \brief Constructor for the How_To_Play_Screen class, initializes the screen with necessary parameters and calls initialization method
        *
        * \param window A shared pointer to the window object where the screen will be displayed
        * \param background_sprite The sprite used as the background for the screen
        * \param pixel_font The font used for text elements on the screen
        */
        How_To_Play_Screen(std::shared_ptr<sf::RenderWindow>, sf::Sprite, sf::Font);

        /** 
        * \fn void How_To_Play_Screen::run()
        * \brief Runs the main loop for the How_To_Play screen, checking for user inputs and updating the display
        *
        * \return void
        */
        void run();
};


#endif