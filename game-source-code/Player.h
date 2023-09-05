#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <memory>

enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Player{
    private:
        int x_position_;
        int y_position_;
        Direction prev_direction_;

        sf::Texture player_texture_;
        sf::Sprite player_sprite_;


        void init_player();



    public:
        Player();
        
        void horizontal_move(Direction direction);
        void vertical_move(Direction direction);
        void render(sf::RenderTarget& target);



};

#endif