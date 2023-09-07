#ifndef PROFESSOR_ASSIGNMENT_H
#define PROFESSOR_ASSIGNMENT_H

#include <SFML/Graphics.hpp>
#include <memory>

class Professor_Assignment {
    private:
        sf::Texture assignment_texture_;
        sf::Sprite assignment_sprite_;

        float assignment_scale_ = 0.5f;
        float assignment_speed_ = 0.2f;

        sf::Vector2f initial_professor_location;
        sf::Vector2f initial_player_location;

        float angle_of_assignment();

    public:
        Professor_Assignment(sf::Texture&, sf::Vector2f, sf::Vector2f);

        sf::Vector2f get_location();
        void move_assignment(float background_movement);
        void draw_assignment(sf::RenderTarget& target);

};


#endif