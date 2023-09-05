#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <memory>

enum class Direction {
    LEFT = -1,
    UP,
    RIGHT,
    DOWN
};

class Player{
    private:
        float x_position_ = 1096.4f;
        float y_position_ = 300.f;
        float scale_player_ = 0.08f;
        float player_speed_ = 0.2f;

        Direction prev_direction_;

        sf::Texture player_texture_;
        sf::Sprite player_sprite_;

        float y_min_pos_ = 120.f;
        float y_max_pos_ = 800.f-this->player_sprite_.getGlobalBounds().height;
        const float x_default_right = 1096.4f;
        const float x_default_left = 150.f;


        void init_player();
        void flip_player();
        bool direction_changed(Direction direction);

    public:
        Player();
        
        void move_player(Direction direction);
        void render(sf::RenderTarget& target);

        float get_x_default_right() const;
        float get_x_default_left() const;

};

#endif