#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

class Game {
    private:
        std::shared_ptr<sf::RenderWindow> window_;
        sf::Texture background_texture_;
        sf::Sprite background_sprite_;

        // CONSTANTS
        const float original_background_width_ = 1920;
        const float original_background_height_ = 1080;
        const float game_width_ = 1400;
        const float game_height_ = 800;
        const float x_scale_ = this->game_width_/this->original_background_width_;
        const float y_scale_ = this->game_height_/this->original_background_height_;

        void _init_window();
        void _init_background();
        void update();
        void render();

    public:
        Game();
        
        void run();
        
};


#endif