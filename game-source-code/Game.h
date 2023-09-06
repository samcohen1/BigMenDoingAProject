#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.h"

class Game {
    private:
        std::shared_ptr<sf::RenderWindow> window_;
        sf::Texture background_texture_;
        sf::Sprite background_sprite_;
        std::unique_ptr<Player> player_;

        // CONSTANTS
        const float original_background_width_ = 1920;
        const float original_background_height_ = 1080;
        const float game_width_ = 1400;
        const float game_height_ = 800;
        const float x_scale_ = this->game_width_/this->original_background_width_;
        const float y_scale_ = this->game_height_/this->original_background_height_;

        float background_base_speed_ = 0.5f;
        float background_location_ = 0.f;
        float background_movement_ = 0.f;

        bool prev_in_edge = false;

        void _init_window();
        void _init_background();
        void _init_player();
        void update();
        void render();

        void handle_player_movement();
        void edge_movement(float, float);
        void internal_movement(float, float);
        void handle_boundary_background_movement();
        void handle_internal_background_movement();

        bool approx_equal(float, float);
        bool approx_innequality(float, float, bool);

    public:
        Game();
        
        void run();
        
};


#endif