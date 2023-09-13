/** \file Throwable.h
    \brief Contains the definitions of the parent type Throwable
 */

#ifndef THROWABLE_H
#define THROWABLE_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Throwable {
    public:
        virtual ~Throwable() = default;

        virtual sf::Vector2f get_location() = 0;
        virtual void move(float) = 0;
        virtual void draw(sf::RenderTarget&) = 0;

        virtual sf::FloatRect get_bounds() = 0;
        virtual sf::FloatRect get_world_bounds() = 0;
        virtual float get_angle() = 0;
};

#endif