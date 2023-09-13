/** \file Professor_Assignment.h
    \brief Contains the definitions of the professor's assignment object
 */

#ifndef PROFESSOR_ASSIGNMENT_H
#define PROFESSOR_ASSIGNMENT_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include "Throwable.h"

class Professor_Assignment : public Throwable {
    private:
        sf::Texture assignment_texture_;
        sf::Sprite assignment_sprite_;

        float assignment_scale_ = 0.05f;
        float assignment_speed_ = 0.2f;
        float angle_ = 0;

        sf::Vector2f initial_professor_location;
        sf::Vector2f initial_player_location;
        sf::FloatRect world_bounds_;

        /** \fn void Professor_Assignment::calculate_angle_of_assignment()
         *  \brief Calculate the angle of the assignment based on its initial positions.
         *  This function calculates the angle of the assignment based on its initial positions, ensuring it points towards the player character.
         */
        void calculate_angle_of_assignment();


        /** \fn int Professor_Assignment::get_relative_side ()
         *  \brief Get the relative side of the assignment compared to the player character.
         *  \return 1 if the assignment is on the right side of the player, -1 if on the left.
         *  This function determines whether the assignment is on the right or left side of the player character.
         */
        int get_relative_side();

    public:

        /** \fn Professor_Assignment::Professor_Assignment(sf::Texture& texture, sf::Vector2f professor_location, sf::Vector2f player_location, sf::FloatRect professor_world_position)
         *  \brief Constructor for Professor_Assignment.
         *  \param texture The texture for the assignment.
         *  \param professor_location The initial position of the professor character.
         *  \param player_location The initial position of the player character.
         *  \param professor_world_position The world bounds of the professor character.
         *  This constructor initializes a Professor_Assignment object with the provided parameters, setting its texture, position, and angle.
         */
        Professor_Assignment(sf::Texture&, sf::Vector2f, sf::Vector2f, sf::FloatRect);


        /** \fn sf::Vector2f Professor_Assignment::get_location()
         *  \brief Get the current position of the assignment.
         *  \return The current position as an SFML Vector2f.
         *  This function returns the current position of the assignment.
         */
        virtual sf::Vector2f get_location() override;


        /** \fn void Professor_Assignment::move(float background_movement)
         *  \brief Move the assignment based on its angle and background movement.
         *  \param background_movement The movement of the game background.
         *  This function updates the position of the assignment based on its angle and background movement.
         */
        virtual void move(float) override;


        /** \fn void Professor_Assignment::draw(sf::RenderTarget &target)
         *  \brief Draw the assignment on the screen.
         *  \param target The render target to draw on.
         *  This function draws the assignment on the provided render target.
         */
        virtual void draw(sf::RenderTarget& target) override;


        /** \fn sf::FloatRect Professor_Assignment::get_bounds()
         *  \brief Get the global bounds of the assignment sprite.
         *  \return The global bounds as an SFML FloatRect.
         *  This function returns the global bounds of the assignment sprite.
         */
        virtual sf::FloatRect get_bounds() override;


        /** \fn sf::FloatRect Professor_Assignment::get_world_bounds()
         *  \brief Get the world bounds of the assignment.
         *  \return The world bounds as an SFML FloatRect.
         *  This function returns the world bounds of the assignment.
         */
        virtual sf::FloatRect get_world_bounds() override;


        /** \fn float Professor_Assignment::get_angle()
         *  \brief Get the angle of the assignment.
         *  \return The angle in radians
         */
        virtual float get_angle() override;
};


#endif