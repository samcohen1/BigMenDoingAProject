#ifndef GAME_H
#define GAME_H

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

        std::vector<std::unique_ptr<Enemy>> enemies_;
        std::vector<std::shared_ptr<Throwable>> throwables_;

        std::vector<std::vector<int>> enemy_vicinities_;

        // PAUSE
        bool paused_ = false;
        bool paused_pressed_ = false;

        // SPLASH
        sf::Texture options_texture;
        sf::Sprite message_screen_sprite;
        sf::Texture message_screen_texture;
        sf::Font font;
        int home_option_selected = 0;
        bool held_down = false;
        bool mouse_over = false;
        bool about_screen_showing = false;
        std::vector<sf::Text> home_options;
        std::vector<sf::Sprite> home_sprites;
        std::vector<sf::Text> messages_;
        bool option_is_selected = false;
        bool is_playing = false;

        // CONSTANTS
        std::vector<sf::Texture> textures;
        const float original_background_width_ = 1920;
        const float original_background_height_ = 1080;
        const float game_width_ = 1400;
        const float game_height_ = 800;
        const float x_scale_ = this->game_width_/this->original_background_width_;
        const float y_scale_ = this->game_height_/this->original_background_height_;

        float background_base_speed_ = 0.5f;
        float background_location_ = 0.f;
        float background_movement_ = 0.f;
        float background_acceleration_ = 0.0003f;

        int professor_cool_down = 0;
        int max_professor_cool_down = 8000;

        float background_movement_tracker = 0.f;

        bool prev_in_edge = false;

        bool shot_held = false;

        void _init_textures();

        void _init_window();
        void _init_background();
        void _init_player();
        void _init_professor();
        void update();
        void render();

        void move_player();
        void edge_movement(float, float);
        void internal_movement(float, float);
        void handle_boundary_background_movement();
        void handle_internal_background_movement();

        void teleport_enemies();
        void move_enemies();
        void render_enemies();
        void move_throwables();
        void render_throwables();
        void erase_enemy(int);
        void erase_throwable(int);

        std::vector<int> get_vicinities(sf::FloatRect, int);
        void bin_vicinities(std::vector<int>, int);

        void handle_collisions();
        void check_bullet_enemy_collision();

        bool approx_equal(float, float);
        bool approx_innequality(float, float, bool);
        void check_player_shoot();
        void check_enemies_shoot();

        // SPLASH
        void _init_home_screen();
        void _init_about_screen();
        void update_home();
        void render_home();

        void update_about();
        void render_about();
        void shift_option_down();
        void shift_option_up();
        void select_using_mouse();

    public:
        Game();
        void run();
        
};


#endif