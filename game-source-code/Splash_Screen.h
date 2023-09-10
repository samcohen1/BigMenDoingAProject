#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

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

        Option option_selected_;

        sf::Texture background_texture_;
        sf::Texture options_texture_;
        sf::Sprite background_sprite_;
        std::vector<sf::Sprite> option_sprites_;

        std::vector<sf::Text> texts_;
        sf::Font pixel_font_;

        const float original_background_width_ = 1920;
        const float original_background_height_ = 1080;
        const float game_width_ = 1400;
        const float game_height_ = 800;
        const float x_scale_ = this->game_width_/this->original_background_width_;
        const float y_scale_ = this->game_height_/this->original_background_height_;

        void update();
        void render();

        void _init_window();
        void _init_background();
        void _init_splash_screen();
        void shift_option_down();
        void shift_option_up();
        void select_using_mouse();

    public:
        Splash_Screen();

        void run();
};

#endif