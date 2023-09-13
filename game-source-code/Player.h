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
        
        bool direction_changed(Direction);
        void init_player(sf::Texture&);
        void flip_player();
        void render_bullets(sf::RenderTarget&, float);
     

    public:
        Player(sf::Texture&);
        
        void move_player_horizontal(Direction);
        void move_player_vertical(Direction, bool);
        void correct_edge_positions();
        void magnetise_player();
        void render(sf::RenderTarget&, float);
        void communicate_position(float);

        void increment_cool_down();

        float get_x_default_right() const;
        float get_x_default_left() const;
        sf::FloatRect get_world_bounds();
        float get_player_speed() const;
        Position get_position();
        Direction get_prev_vertical_direction();
        std::vector<std::shared_ptr<Bullet>> get_bullets() const;
        void shoot_bullet(sf::Texture&);
        void erase_bullet(int);
        void erase_bullet(std::shared_ptr<Bullet>);
};

#endif