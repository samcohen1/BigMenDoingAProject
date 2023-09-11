#ifndef PROFESSOR_ASSIGNMENT_H
#define PROFESSOR_ASSIGNMENT_H

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

        void calculate_angle_of_assignment();
        int get_relative_side();

    public:
        Professor_Assignment(sf::Texture&, sf::Vector2f, sf::Vector2f, sf::FloatRect);

        virtual sf::Vector2f get_location() override;
        virtual void move(float) override;
        virtual void draw(sf::RenderTarget& target) override;

        virtual sf::FloatRect get_bounds() override;
        virtual sf::FloatRect get_world_bounds() override;
};


#endif