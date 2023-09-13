/** \file Professor.h
    \brief Contains the definitions of the professor
 */

#ifndef PROFESSOR_H
#define PROFESSOR_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Bullet.h"
#include "Professor_Assignment.h"
#include "Throwable.h"
#include "Enemy.h"

static int num_professors_{0};

class Professor : public Enemy {
    private:
        float scale_professor_ = 0.1f;

        Direction prev_direction_;

        sf::Texture professor_texture_;
        sf::Sprite professor_sprite_;

        float y_min_pos_ = 140.f;
        float y_max_pos_ = 760.f-this->professor_sprite_.getGlobalBounds().height;

        int frames_since_spawn = 0;
        float vertical_speed_ = 0.02f;
        float horizontal_speed_ = 0.03f;
        float y_shift = 0;
        float movement_amplitude = 0;
        float movement_period = 0;
        float movement_gradient = 0;

        float initial_x_position = 0;
        float initial_y_position = 0;
        float prev_y_position = 0;
        float y_direction = 1;
        sf::Vector2f world_position;
        sf::FloatRect world_bounds;

        int current_cool_down = 0;
        int max_cool_down = 1000;
        int is_dying_counter = 0;
        bool is_dead = false;

        /** \fn void Professor::init_professor(sf::Texture& texture)
         *  \brief Initialize the Professor object.
         *  \param texture The texture for the Professor's sprite.
         *  This function sets up the Professor's initial properties, including its world position.
         */
        void init_professor(sf::Texture&);

        
        /** \fn void Professor::flip_professor()
         *  \brief Flip the Professor's sprite to change direction.
         *  This function flips the Professor's sprite horizontally to change its direction of movement.
         */
        void flip_professor();


        /** \fn void Professor::face_player(sf::Vector2f player_position)
         *  \brief Make the Professor face the player character.
         *  \param player_position The position of the player character.
         *  This function flips the Professor's sprite to make it face the player character.
         */
        void face_player(sf::Vector2f);
        void render_assignments(sf::RenderTarget& target, float);


        
        /** \fn bool Professor::direction_changed(Direction direction)
         *  \brief Check if the direction of the Professor has changed.
         *  \param direction The new direction to check against.
         *  \return True if the direction has changed; false otherwise.
         *  This function checks if the direction of the Professor has changed compared to the provided direction.
         */
        bool direction_changed(Direction);


        /** \fn float Professor::movement_function()
         *  \brief Calculate the movement function for the Professor.
         *  \return The calculated movement value.
         *  This function calculates the vertical movement of the Professor character based on amplitude, period, and gradient.
         */
        float movement_function();

        /** \fn float Professor::move_vertical()
         *  \brief Calculate the movement for the Professor's y-component.
         *  This function calculates the vertical movement of the Professor character and moves him.
         */
        void move_vertical();

        /** \fn float Professor::move_horizontal()
         *  \brief Calculate the movement for the Professor's x-component. 
         *  This function calculates the horizontal movement of the Professor character and moves him.
         */
        void move_horizontal(float, float);

    public:

        /** \class Professor
         *  \brief Represents a Professor character in the game.
         *  \param texture The texture for the Professor's sprite.
         *  This constructor initializes a Professor character with the given texture.
         */
        Professor(sf::Texture&);


        /** \class Professor
         *  \brief Destructor for the Professor class.
         */
        ~Professor();


        /** \fn int Professor::get_num_professors()
         *  \brief Get the number of Professor objects.
         *  \return The number of Professor objects.
         */
        static int get_num_professors();
        

        /** \fn void Professor::move(float background_location, sf::Vector2f player_position)
         *  \brief Move the Professor character.
         *  \param background_movement The current movement of the game background.
         *  \param background_location The location of the game background.
         *  \param player_position The position of the player character.
         *  This function updates the position of the Professor character based on its movement properties and player position.
         */
        virtual void move(float, float, sf::Vector2f) override;


        /** \fn void Professor::render(sf::RenderTarget &target, float background_movement)
         *  \brief Render the Professor on the game screen.
         *  \param target The render target to draw on.
         *  \param background_movement The movement of the game background.
         *  This function renders the Professor character on the game screen.
         */
        virtual void render(sf::RenderTarget& target, float) override;


        /** \fn void Professor::destroy()
         *  \brief Destroy the Professor character.
         *  This function handles the destruction of the Professor character, including the sprite animation.
         */
        virtual void destroy() override;

        /** \fn void Professor::increment_cool_down()
         *  \brief Increment the cooldown for the Professor's attacks.
         *  This function increments the cooldown timer for the Professor's attacks.
         */
        virtual void increment_cool_down() override;


        /** \fn sf::Vector2f Professor::get_location()
         *  \brief Get the current location of the Professor character.
         *  \return The current location of the Professor character.
         *  This function returns the current location of the Professor character.
         */
        virtual sf::Vector2f get_location() override;  


        /** \fn sf::FloatRect Professor::get_world_bounds()
         *  \brief Get the bounding box of the Professor in the game world.
         *  \return The bounding box of the Professor in the game world.
         *  This function returns the bounding box of the Professor character in the game world.
         */
        virtual sf::FloatRect get_world_bounds() override;  


        /** \fn bool Professor::get_is_dead()
         *  \brief Check if the Professor character is dead.
         *  \return True if the Professor character is dead; false otherwise.
         *  This function checks whether the Professor character is dead.
         */
        virtual bool get_is_dead() override;


        /** \fn bool Professor::is_dying()
         *  \brief Check if the Professor character is in the process of dying.
         *  \return True if the Professor character is in the process of dying; false otherwise.
         *  This function checks if the Professor character is in the process of dying.
         */
        virtual bool is_dying() override;


        /** \fn std::shared_ptr<Throwable> Professor::shoot_throwable(sf::Texture& texture, sf::Vector2f player_position)
         *  \brief Create and return a Throwable object.
         *  \param texture The texture for the Throwable.
         *  \param player_position The position of the player character.
         *  \return A shared pointer to the created Throwable object.
         *  This function creates and returns a Throwable object, representing an attack by the Professor character.
         */
        virtual std::shared_ptr<Throwable> shoot_throwable(sf::Texture&, sf::Vector2f) override;
};

#endif