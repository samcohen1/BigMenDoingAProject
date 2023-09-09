#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Bullet.h"
#include "Professor_Assignment.h"
#include "Throwable.h"

class Enemy {
    public:
        virtual ~Enemy() = default;

        virtual void move(float, sf::Vector2f) = 0;
        virtual void render(sf::RenderTarget&, float) = 0;
        virtual void destroy() = 0;
        virtual void increment_cool_down() = 0;
        virtual std::shared_ptr<Throwable> shoot_throwable(sf::Texture&, sf::Vector2f) = 0;
        
        virtual sf::Vector2f get_location() = 0;
        virtual sf::FloatRect get_bounds() = 0; 
        virtual sf::FloatRect get_world_bounds() = 0;

        virtual bool get_is_dead() = 0;
        virtual bool is_dying() = 0;    
};

#endif