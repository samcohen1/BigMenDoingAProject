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
        std::shared_ptr<sf::RenderWindow> window_;

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

        bool key_held_down_ = true;

        void _init_how_to_play_screen();
        void update();
        void render();
        bool is_back_button_hovered();

    public:
        How_To_Play_Screen(std::shared_ptr<sf::RenderWindow>, sf::Sprite, sf::Font);
        void run();
};


#endif