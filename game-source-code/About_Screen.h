#ifndef ABOUT_SCREEN_H
#define ABOUT_SCREEN_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class About_Screen {
    private:
        std::shared_ptr<sf::RenderWindow> window_;

        sf::Texture box_texture_;
        sf::Sprite box_sprite_;

        sf::Texture back_button_texture_;
        sf::Sprite back_button_sprite_;

        sf::Sprite background_sprite_;

        sf::Font pixel_font_;
        std::vector<sf::Text> texts_;

        bool key_held_down_ = true;

        void _init_about_screen();
        void update();
        void render();
        bool is_back_button_hovered();

    public:
        About_Screen(std::shared_ptr<sf::RenderWindow>, sf::Sprite, sf::Font);
        void run();
};


#endif