#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <memory>

enum class Direction {
    RIGHT = -1,
    UP,
    LEFT,
    DOWN
};

struct Position {
    float x_left;
    float x_right;
    float y;
};

class Player{
    private:

        float scale_player_ = 0.08f;
        float player_speed_ = 0.01f;
        float vertical_speed_ = 0.2f;
        float flip_base_speed_ = 0.15f;
        float acceleration_ = 0.0008f;

        Direction prev_direction_;

        sf::Texture player_texture_;
        sf::Sprite player_sprite_;

        float y_min_pos_ = 120.f;
        float y_max_pos_ = 800.f-this->player_sprite_.getGlobalBounds().height;
        float default_y_ = 300.f;
        const float x_default_right_ = 1250.f;
        const float x_default_left_ = 150.f;

        void init_player();
        void flip_player();
        bool direction_changed(Direction);

    public:
        Player();
        
        void move_player_horizontal(Direction direction);
        void move_player_vertical(Direction direction);
        void edge_player_movement(Direction);
        void magnatise_player();
        void render(sf::RenderTarget& target);

        float get_x_default_right() const;
        float get_x_default_left() const;
        float get_player_speed() const;
        Position get_position();
};

#endif