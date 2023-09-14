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
    /************************************************************************************
    **                             INITIALISATION CONSTANTS                            **
    ************************************************************************************/
        // TEXTURES AND SPRITES
        sf::Texture bullet_texture_;
        sf::Sprite bullet_sprite_;

        // INITIALISATION
        float x_location_;
        float y_location_;
        sf::FloatRect world_bounds_;
        Direction direction_;
        float bullet_scale_ = 0.3f;
        float bullet_movement = 1.f;
    //***********************************************************************************

    public:
        /**
        * \fn Bullet::Bullet(float x_location, float y_location, Direction direction, sf::Texture& texture, sf::FloatRect player_world_position)
        * \brief This constructor initializes a Bullet object with the specified location, direction, texture, and world position.
        * 
        * \param x_location The initial x-coordinate of the bullet.
        * \param y_location The initial y-coordinate of the bullet.
        * \param direction The direction the bullet is fired in.
        * \param texture The texture applied to the bullet sprite.
        * \param player_world_position The world boundaries within which the bullet operates.
        * 
        */
        Bullet(float x_location, float y_location, Direction, sf::Texture&, sf::FloatRect);

        /**
        * \fn void Bullet::move(float background_movement)
        * \brief This method moves the bullet sprite based on its direction and adjusts its world bounds accordingly.
        * 
        * \param background_movement The amount of movement to consider for the background.
        * 
        */
        void move(float);

        /**
        * \fn void Bullet::draw(sf::RenderTarget& target)
        * \brief This method draws the bullet sprite onto the target render target.
        * 
        * \param target The render target to which the bullet sprite is drawn.
        * 
        */
        void draw(sf::RenderTarget& target);

        /**
        * \fn sf::FloatRect Bullet::get_world_bounds()
        * \brief This method retrieves the current world bounds within which the bullet operates.
        * 
        * \return Returns the current world bounds as a floating point rectangle.
        * 
        */
        sf::FloatRect get_world_bounds();

        /**
        * \fn sf::Vector2f Bullet::get_location()
        * \brief This method retrieves the current position of the bullet sprite.
        * 
        * \return Returns the current position of the bullet sprite as a vector of floats.
        * 
        */
        sf::Vector2f get_location();
};

#endif