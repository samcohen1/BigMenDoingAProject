#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Bullet.h"

class Professor {
    private:

        float scale_Professor_ = 0.08f;
        float Professor_speed_ = 0.01f;
        float vertical_speed_ = 0.2f;
        float flip_base_speed_ = 0.15f;
        float acceleration_ = 0.0008f;
        float edge_acceleration_ = 0.0005f;
        float Professor_edge_speed = 0.5f;

        Direction prev_direction_;

        sf::Texture Professor_texture_;
        sf::Sprite Professor_sprite_;

        float y_min_pos_ = 120.f;
        float y_max_pos_ = 800.f-this->Professor_sprite_.getGlobalBounds().height;

        int current_cool_down = 400;
        int max_cool_down = 400;

        // std::vector<std::shared_ptr<Bullet>> bullets_;

        void init_Professor();
        void flip_Professor();
        bool direction_changed(Direction);

    public:
        Professor();
        
        void move_Professor_horizontal(Direction direction);
        void move_Professor_vertical(Direction direction);
        void edge_Professor_movement(Direction);
        void edge_decelerate();
        void correct_edge_positions();
        void render(sf::RenderTarget& target);

        void render_bullets(sf::RenderTarget& target, float);
        void increment_cool_down();

        float get_Professor_speed() const;
        sf::Vector2f get_location();     
        // std::vector<std::shared_ptr<Bullet>> get_bullets() const;
        void shoot_bullet(sf::Texture&);
        void erase_bullet(int position);
};

#endif