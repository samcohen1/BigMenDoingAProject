#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <memory>

enum class Direction {
    RIGHT = -1,
    UP,
    LEFT,
    DOWN
};

class Bullet {
    private:
        sf::Texture bullet_texture_;
        sf::Sprite bullet_sprite_;

        float x_location_;
        float y_location_;
        float bullet_scale_ = 0.3f;
        float bullet_movement = 1.f;
        Direction direction_;

    public:
        Bullet(float x_location, float y_location, Direction, sf::Texture&);

        sf::Vector2f get_location();        
        float get_bullet_width();

        void move_bullet(float background_movement);
        void draw_bullet(sf::RenderTarget& target);
};

#endif