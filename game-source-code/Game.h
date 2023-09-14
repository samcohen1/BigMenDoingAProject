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

// This enum refers to which texture is being refered to when initialising objects of certain types
enum class Textures{
    BULLET = 0,
    PLAYER_SHEET,
    PROFESSOR_SHEET,
    PROFESSOR_ASSIGNMENT
};

class Game {
    private:
    /************************************************************************************
    **                      WINDOW AND GAME SPECIFICATIONS                            **
    ************************************************************************************/
        std::shared_ptr<sf::RenderWindow> window_;
        const float original_background_width_;
        const float original_background_height_;
        const float game_width_;
        const float game_height_;
        const float x_scale_;
        const float y_scale_;

        // BACKGROUND
        float background_base_speed_ = 0.5f;
        float background_location_ = 0.f;
        float background_movement_ = 0.f;
        float background_acceleration_ = 0.0003f;
        float background_movement_tracker = 0.f;

        // PAUSE
        bool paused_ = false;
        bool paused_pressed_ = false;
    //***********************************************************************************

    /************************************************************************************
    **                             TEXTURES AND SPRITES                                **
    ************************************************************************************/
        std::vector<sf::Texture> textures;
        sf::Texture background_texture_;
        sf::Sprite background_sprite_;
    //***********************************************************************************

    /************************************************************************************
    **                       PLAYER, ENEMIES, AND THROWABLES                           **
    ************************************************************************************/
        std::unique_ptr<Player> player_;
        std::vector<std::shared_ptr<Enemy>> enemies_;
        std::vector<std::shared_ptr<Throwable>> throwables_;
        std::vector<std::vector<std::shared_ptr<Enemy>>> actual_enemy_vicinities_;

        // PPLAYER
        bool shot_held = false;

        // PROFESSOR
        int professor_cool_down = 0;
        int max_professor_cool_down = 100;
    //***********************************************************************************



        /**
        * \fn void Game::_init_textures()
        * \brief This function initializes various textures used in the game including player, professor and bullet textures.
        * 
        * \return No return value (void function), but exits early if any texture fails to load.
        */
        void _init_textures();

        /**
        * \fn void Game::_init_background()
        * \brief This function initializes the background texture and sets up its sprite with appropriate properties.
        * 
        * \return No return value (void function), but exits early if the background texture fails to load.
        */
        void _init_background();

        /**
        * \fn void Game::_init_player()
        * \brief This function initializes the player object with the appropriate texture.
        *
        * \return No return value (void function).
        */
        void _init_player();

        /**
        * \fn void Game::_init_professor()
        * \brief This function initializes a new professor enemy and adds it to the enemies list.
        *
        * \return No return value (void function).
        */
        void _init_professor();

        /**
        * \fn void Game::update()
        * \brief This function handles the game's updates including event polling and various gameplay updates based on game state.
        * 
        * \return No return value (void function).
        */
        void update();

        /**
        * \fn void Game::render()
        * \brief This function handles rendering various game elements like background, player, enemies and throwables on the game window.
        *
        * \return No return value (void function).
        */
        void render();

        /** 
        * \fn void Game::move_player()
        * \brief Controls the player's movement by considering edge cases, background location, and keyboard inputs
        * 
        * \return void
        */
        void move_player();

        /** 
        * \fn void Game::internal_movement(float x_right, float x_left)
        * \brief Handles the internal movement of the player and background based on various conditions and keyboard inputs
        *
        * \param x_right The right boundary for player movement
        * \param x_left The left boundary for player movement
        * \return void
        */
        void internal_movement(float, float);

        /** 
        * \fn void Game::handle_boundary_background_movement()
        * \brief Handles the background movement within specified boundaries based on background location and movement variables
        * 
        * \return void
        */
        void handle_boundary_background_movement();

        /** 
        * \fn void Game::handle_internal_background_movement()
        * \brief Handles the internal background movement based on player's speed and keyboard inputs
        * 
        * \return void
        */
        void handle_internal_background_movement();

        /** 
        * \fn void Game::wrap()
        * \brief Controls background movement to wrap back around creating infinite scrolling
        * 
        * \return void
        */
        void wrap();

        /**
        * \fn void Game::render_enemies()
        * \brief This function iterates over all enemies and triggers their render function.
        *
        * \return No return value (void function).
        */
        void render_enemies();

        /**
        * \fn void Game::move_throwables()
        * \brief This function handles the movement of throwables in the game, removing them if they move out of the allowable area.
        *
        * \return No return value (void function).
        */
        void move_throwables();

        /**
        * \fn void Game::render_throwables()
        * \brief This function iterates over all throwables and triggers their draw function.
        *
        * \return No return value (void function).
        */
        void render_throwables();

        /**
        * \fn std::vector<int> Game::get_vicinities(sf::FloatRect rect, int position)
        * \brief This function calculates the vicinity indices based on a given rectangular area and position.
        *
        * \param rect A float rectangle that defines the area.
        * \param position The position index.
        * 
        * \return A vector containing the unique vicinity indices.
        */
        std::vector<int> get_vicinities(sf::FloatRect, int);

        /**
        * \fn void Game::bin_vicinities(std::vector<int> vicinities, int position)
        * \brief This function updates the enemy vicinities based on the given position and vicinity indices.
        *
        * \param vicinities A vector containing the indices of the vicinities.
        * \param enemy pointer to an Enemy object
        * 
        * \return No return value (void function).
        */
        void bin_vicinities(std::vector<int>, std::shared_ptr<Enemy>);

        /**
        * \fn void Game::handle_collisions()
        * \brief This function is a wrapper for various collision checking functions.
        *
        * \return No return value (void function).
        */
        void handle_collisions();

        /**
        * \fn void Game::check_bullet_enemy_collision()
        * \brief This function checks for collisions between bullets and enemies, destroying any entities involved in a collision.
        *
        * \return No return value (void function).
        */
        void check_bullet_enemy_collision();

        /** 
        * \fn void Game::check_player_shoot()
        * \brief Handles the player's shooting actions based on keyboard inputs
        * 
        * \return void
        */
        void check_player_shoot();

        /** 
        * \fn void Game::check_enemies_shoot()
        * \brief Checks and handles shooting actions of all enemies
        * 
        * \return void
        */
        void check_enemies_shoot();

    public:
        /**
        * \fn Game::Game(std::shared_ptr<sf::RenderWindow> window, float original_background_width, float original_background_height, float game_width, float game_height, float x_scale, float y_scale)
        * \brief Constructor of the Game class which initializes several game elements such as background, textures, player and professor.
        * 
        * \param window A shared pointer to the RenderWindow object that represents the window where the game is rendered.
        * \param original_background_width The original width of the background.
        * \param original_background_height The original height of the background.
        * \param game_width The width of the game area.
        * \param game_height The height of the game area.
        * \param x_scale The scale factor along the X-axis.
        * \param y_scale The scale factor along the Y-axis.
        * 
        * \return No return value (constructor).
        */
        Game(std::shared_ptr<sf::RenderWindow>, float, float, float, float, float, float);
        
        /**
        * \fn void Game::run()
        * \brief This function contains the main loop where update and render functions are called continuously while the window is open.
        *
        * \return No return value (void function).
        */
        void run();

        /**
        * \fn void Game::teleport_enemies()
        * \brief This function handles the logic for spawning new professor enemies at regular intervals until a maximum limit is reached.
        *
        * \return No return value (void function).
        */
        void teleport_enemies(int);

        /**
        * \fn void Game::move_enemies()
        * \brief This function handles the movement and status updates for each enemy in the game. It also dynamically bins each enemy into the correct vacinity used for collision detection
        *
        * \return No return value (void function).
        */
        void move_enemies();

         /**
        * \fn void Game::erase_enemy(int position)
        * \brief This function removes an enemy from the enemies vector at the specified position.
        *
        * \param position The index of the enemy to be removed.
        * \return No return value (void function).
        */
        void erase_enemy(int);

        /** 
        * \fn void Game::erase_throwable(int position)
        * \brief Erases a throwable object from the vector of throwables at the specified position
        *
        * \param position The position of the throwable object to erase
        * \return void
        */
        void erase_throwable(int);

        /** 
        * \fn std::vector<std::shared_ptr<Enemy>> get_enemies()
        * \brief Returns a vector of shared pointers to the enemies in the current game
        *
        * \return std::vector<std::shared_ptr<Enemy>>
        */
        std::vector<std::shared_ptr<Enemy>> get_enemies();

        /** 
        * \fn  std::vector<std::shared_ptr<Throwable>> get_throwables()
        * \brief Returns a vector of shared pointers to the throwables in the current game
        *
        * \return std::vector<std::shared_ptr<Throwable>>
        */
        std::vector<std::shared_ptr<Throwable>> get_throwables();
};


#endif