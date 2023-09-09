#ifndef THROWABLE_H
#define THROWABLE_H

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
};

#endif