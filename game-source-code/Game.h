/** \file Game.h
    \brief Contains the definitions of the game
 */

#ifndef GAME_H
#define GAME_H

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <ctime>

#include "Player.h"
#include "Bullet.h"
#include "Professor.h"
#include "Professor_Assignment.h"

#include "Enemy.h"
#include "Throwable.h"

enum class Textures{
    BULLET = 0,
    PLAYER_SHEET,
    PROFESSOR_SHEET,
    PROFESSOR_ASSIGNMENT
};

class Game {
    private:
        std::shared_ptr<sf::RenderWindow> window_;
        sf::Texture background_texture_;
        sf::Sprite background_sprite_;
        std::unique_ptr<Player> player_;

        std::vector<std::shared_ptr<Enemy>> enemies_;
        std::vector<std::shared_ptr<Throwable>> throwables_;

        std::vector<std::vector<std::shared_ptr<Enemy>>> actual_enemy_vicinities_;

        // PAUSE
        bool paused_ = false;
        bool paused_pressed_ = false;

        // CONSTANTS
        std::vector<sf::Texture> textures;

        const float original_background_width_;
        const float original_background_height_;
        const float game_width_;
        const float game_height_;
        const float x_scale_;
        const float y_scale_;

        float background_base_speed_ = 0.5f;
        // float background_base_speed_ = 5.f;
        float background_location_ = 0.f;
        float background_movement_ = 0.f;
        float background_acceleration_ = 0.0003f;

        int professor_cool_down = 0;
        int max_professor_cool_down = 100;

        float background_movement_tracker = 0.f;


        bool shot_held = false;

        void _init_textures();

        void _init_window();
        void _init_background();
        void _init_player();
        void _init_professor();
        void update();
        void render();

        void move_player();
        void internal_movement(float, float);
        void handle_boundary_background_movement();
        void handle_internal_background_movement();
        void wrap();

        void teleport_enemies();
        void move_enemies();
        void render_enemies();
        void move_throwables();
        void render_throwables();
        void erase_enemy(int);
        void erase_throwable(int);

        std::vector<int> get_vicinities(sf::FloatRect, int);
        void bin_vicinities(std::vector<int>, int);
        void bin_vicinities(std::vector<int>, std::shared_ptr<Enemy>);

        void handle_collisions();
        void check_bullet_enemy_collision();

        void check_player_shoot();
        void check_enemies_shoot();

    public:
        Game(std::shared_ptr<sf::RenderWindow>, float, float, float, float, float, float);
        Game();
        void run();
        
};


#endif