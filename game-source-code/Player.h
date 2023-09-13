/** \file Player.h
    \brief Contains the definitions of the player
 */

#ifndef PLAYER_H
#define PLAYER_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Bullet.h"

struct Position {
    float x_left;
    float x_right;
    float y;
};

class Player {
    private:
        float scale_player_ = 0.08f;
        float player_speed_ = 0.01f;
        float vertical_speed_ = 0.2f;
        float vertical_base_speed = this->vertical_speed_;
        float flip_base_speed_ = 0.15f;
        float acceleration_ = 0.0008f;
        float vertical_acceleration_ = 0.0004f;

        Direction prev_direction_;
        Direction prev_vertical_direction_;

        sf::Texture player_texture_;
        sf::Sprite player_sprite_;

        float y_min_pos_ = 120.f;
        float y_max_pos_ = 800.f-this->player_sprite_.getGlobalBounds().height;
        float default_y_ = 300.f;
        const float x_default_right_ = 1250.f;
        const float x_default_left_ = 150.f;
        sf::FloatRect world_bounds_;

        int current_cool_down = 400;
        int max_cool_down = 400;

        std::vector<std::shared_ptr<Bullet>> bullets_;
        

        /** \fn bool Player::direction_changed(Direction direction)
         *  \brief Check if the direction of the player character has changed.
         *  \param direction The new direction to check against.
         *  \return True if the direction has changed; false otherwise.
         *  This function checks if the direction of the player character has changed compared to the provided direction.
         */
        bool direction_changed(Direction);


        /** \fn void Player::init_player(sf::Texture& texture)
         *  \brief Initialize the player character.
         *  \param texture The texture for the player's sprite.
         *  This function initializes the player character with the given texture and sets its initial properties.
         */
        void init_player(sf::Texture&);


        /** \fn void Player::flip_player()
         *  \brief Flip the player character to change direction.
         *  This function flips the player's sprite horizontally to change its facing direction.
         */
        void flip_player();
        

        /** \fn void Player::render_bullets(sf::RenderTarget &target, float background_movement)
         *  \brief Render the player's bullets on the screen.
         *  \param target The render target to draw on.
         *  \param background_movement The movement of the game background.
         *  This function renders the player's bullets on the screen.
         */
        void render_bullets(sf::RenderTarget&, float);
     

    public:

        /** \class Player
         *  \brief Represents the player character in the game.
         */
        Player(sf::Texture&);
        

        /** \fn void Player::move_player_horizontal(Direction direction)
         *  \brief Move the player character horizontally.
         *  \param direction The direction of movement (LEFT or RIGHT).
         *  This function updates the horizontal position of the player character using its direction.
         */
        void move_player_horizontal(Direction);


        /** \fn void Player::move_player_vertical(Direction direction, bool pressed)
         *  \brief Move the player character vertically.
         *  \param direction The direction of movement (UP or DOWN).
         *  \param pressed True if the movement key is pressed; false otherwise.
         *  This function updates the vertical position of the player character based on its direction and key press status.
         */
        void move_player_vertical(Direction, bool);


        /** \fn void Player::correct_edge_positions()
         *  \brief Correct the player character's position at the screen edge.
         *  This function corrects the position of the player character to the edge of the screen.
         */
        void correct_edge_positions();


        /** \fn void Player::magnetise_player()
         *  \brief Magnetize the player character towards the edge.
         *  This function magnetizes the player character towards the edge when it's in the middle.
         */
        void magnetise_player();


        /** \fn void Player::render(sf::RenderTarget &target, float background_movement)
         *  \brief Render the player character and bullets on the screen.
         *  \param target The render target to draw on.
         *  \param background_movement The movement of the game background.
         *  This function renders the player character and bullets on the screen.
         */
        void render(sf::RenderTarget&, float);


        /** \fn void Player::communicate_position(float background_location)
         *  \brief Communicate the player character's position.
         *  \param background_location The location of the game background.
         *  This function communicates the position of the player character to the game, taking into account the background location.
         */
        void communicate_position(float);


        /** \fn void Player::increment_cool_down()
         *  \brief Increment the shooting cooldown for the player character.
         *  This function increments the shooting cooldown timer for the player character.
         */
        void increment_cool_down();


        /** \fn float Player::get_x_default_right() const
         *  \brief Get the default right position of the player character.
         *  \return The default right position.
         *  This function returns the default right position of the player character.
         */
        float get_x_default_right() const;


        /** \fn float Player::get_x_default_left() const
         *  \brief Get the default left position of the player character.
         *  \return The default left position.
         *  This function returns the default left position of the player character.
         */
        float get_x_default_left() const;


        /** \fn sf::FloatRect Player::get_world_bounds()
         *  \brief Get the world bounds of the player character.
         *  \return The world bounds as an SFML FloatRect.
         *  This function returns the world bounds of the player character.
         */
        sf::FloatRect get_world_bounds();


        /** \fn float Player::get_player_speed() const
         *  \brief Get the speed of the player character.
         *  \return The speed of the player character.
         *  This function returns the speed of the player character.
         */
        float get_player_speed() const;


        /** \fn Position Player::get_position()
         *  \brief Get the current position of the player character.
         *  \return The current position.
         *  This function returns the current position of the player character.
         */
        Position get_position();


        /** \fn Direction Player::get_prev_vertical_direction()
         *  \brief Get the previous vertical direction of the player character.
         *  \return The previous vertical direction (UP or DOWN).
         *  This function returns the previous vertical direction of the player character.
         */
        Direction get_prev_vertical_direction();


        /** \fn std::vector<std::shared_ptr<Bullet>> Player::get_bullets() const
         *  \brief Get the player's bullets.
         *  \return A vector of shared pointers to the player's bullets.
         *  This function returns a vector of shared pointers to the player's bullets.
         */
        std::vector<std::shared_ptr<Bullet>> get_bullets() const;


        /** \fn void Player::shoot_bullet(sf::Texture& texture)
         *  \brief Shoot a bullet from the player character.
         *  \param texture The texture for the bullet.
         *  This function allows the player character to shoot a bullet.
         */
        void shoot_bullet(sf::Texture&);


        /** \fn void Player::erase_bullet(int position)
         *  \brief Erase a bullet from the player's bullets.
         *  \param position The position of the bullet to erase.
         *  This function erases a bullet from the player's bullets vector.
         */
        void erase_bullet(int);


        /** \fn void Player::erase_bullet(std::shared_ptr<Bullet>)
         *  \brief Erase a bullet from the player's bullets.
         *  \param bullet a pinter to the bullet to erase.
         *  This function erases a bullet from the player's bullets vector.
         */
        void erase_bullet(std::shared_ptr<Bullet>);
};

#endif