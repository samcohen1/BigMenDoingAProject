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
    float x_left = 1172.52f;
    float x_right = 1250.0f;
    float y = 300.0f;
};

class Player{
    private:

        Position position_;
        float scale_player_ = 0.08f;
        float player_speed_ = 0.01f;
        float vertical_speed_ = 0.2f;
        float base_speed_ = 0.15f;
        float acceleration_ = 0.0008f;

        Direction prev_direction_;

        sf::Texture player_texture_;
        sf::Sprite player_sprite_;

        float y_min_pos_ = 120.f;
        float y_max_pos_ = 800.f-this->player_sprite_.getGlobalBounds().height;
        const float x_default_right = 1250.f;
        const float x_default_left = 150.f;


        void init_player();
        void flip_player();
        void update_position(Direction);
        bool direction_changed(Direction direction);

    public:
        Player();
        
        void move_player(Direction direction);
        void render(sf::RenderTarget& target);

        float get_x_default_right() const;
        float get_x_default_left() const;
        Position get_position();
};

#endif