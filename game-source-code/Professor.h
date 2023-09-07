#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Bullet.h"

class Professor {
    private:

        float scale_professor_ = 0.1f;
        float flip_base_speed_ = 0.15f;
        float acceleration_ = 0.0008f;
        float edge_acceleration_ = 0.0005f;
        float professor_edge_speed = 0.5f;

        Direction prev_direction_;

        sf::Texture professor_texture_;
        sf::Sprite professor_sprite_;

        float y_min_pos_ = 120.f;
        float y_max_pos_ = 700.f-this->professor_sprite_.getGlobalBounds().height;

        int frames_since_spawn = 0;
        float vertical_speed_ = 0.02f;
        float horizontal_speed_ = 0.3f;
        float y_shift = 0;
        float movement_amplitude = 0;
        float movement_period = 0;
        float movement_gradient = 0;

        float initial_x_position = 0;
        float initial_y_position = 0;
        float world_position = 0;

        int current_cool_down = 400;
        int max_cool_down = 400;
        

        // std::vector<std::shared_ptr<Bullet>> bullets_;

        void init_professor(sf::Texture&);
        void flip_professor();
        bool direction_changed(Direction);
        float movement_function();

    public:
        Professor(sf::Texture&);
        
        void move_professor(float);
        void edge_professor_movement(Direction);
        void edge_decelerate();
        void correct_edge_positions();
        void render(sf::RenderTarget& target);

        void render_bullets(sf::RenderTarget& target, float);
        void increment_cool_down();

        float get_professor_speed() const;
        sf::Vector2f get_location();     
        // std::vector<std::shared_ptr<Bullet>> get_bullets() const;
        void shoot_bullet(sf::Texture&);
        void erase_bullet(int position);
};

#endif