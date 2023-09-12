/** \file Bullet.h
    \brief Contains the definitions of the player's bullet
 */

#ifndef BULLET_H
#define BULLET_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>

#include "Throwable.h"

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
        sf::FloatRect world_bounds_;
        float bullet_scale_ = 0.3f;
        float bullet_movement = 1.f;
        Direction direction_;

    public:
        Bullet(float x_location, float y_location, Direction, sf::Texture&, sf::FloatRect);

        sf::Vector2f get_location();
        void move(float);
        void draw(sf::RenderTarget& target);
        sf::FloatRect get_world_bounds();
        sf::FloatRect get_bounds();
};

#endif