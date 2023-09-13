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

        float y_min_pos_ = 100.f;
        float y_max_pos_ = 800.f-this->professor_sprite_.getGlobalBounds().height;

        int frames_since_spawn = 0;
        float vertical_speed_ = 0.02f;
        // float horizontal_speed_ = 0.03f;
        float horizontal_speed_ = 0.1f;
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

        void init_professor(sf::Texture&);
        void flip_professor();
        void face_player(sf::Vector2f);
        void render_assignments(sf::RenderTarget& target, float);
        bool direction_changed(Direction);
        float movement_function();

        void move_vertical();
        void move_horizontal(float, float);

    public:
        Professor(sf::Texture&);
        ~Professor();

        static int get_num_professors();
        
        virtual void move(float, float, sf::Vector2f) override;
        virtual void render(sf::RenderTarget& target, float) override;
        virtual void destroy() override;

        virtual void increment_cool_down() override;

        virtual sf::Vector2f get_location() override;  
        virtual sf::FloatRect get_world_bounds() override;  
        virtual bool get_is_dead() override;
        virtual bool is_dying() override;

        virtual std::shared_ptr<Throwable> shoot_throwable(sf::Texture&, sf::Vector2f) override;
};

#endif