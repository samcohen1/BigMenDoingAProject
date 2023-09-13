/** \file Game.cpp
    \brief Contains the implementation of the game
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>

#include "Game.h"
#include "Bullet.h"
#include "Professor.h"
#include "Player.h"
#include "Professor_Assignment.h"
#include "Enemy.h"
#include "Throwable.h"

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
Game::Game(std::shared_ptr<sf::RenderWindow> window, float original_background_width, float original_background_height, float game_width, float game_height, float x_scale, float y_scale) : 
        window_(window), original_background_width_(original_background_width), original_background_height_(original_background_height), game_width_(game_width), game_height_(game_height), x_scale_(x_scale), y_scale_(y_scale) {
    this->_init_background();
    this->_init_textures();
    this->_init_player();
}

/**
 * \fn void Game::update()
 * \brief This function handles the game's updates including event polling and various gameplay updates based on game state.
 * 
 * \return No return value (void function).
 */
void Game::update() {
    sf::Event event;
    while (this->window_->pollEvent(event)) if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) this->window_->close();
    // Handle Pause
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !this->paused_pressed_) {
        this->paused_ = !this->paused_;
        this->paused_pressed_ = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) this->paused_pressed_ = false;
    if (this->paused_) return;

    this->move_player();
    this->player_->communicate_position(this->background_location_);
    this->check_player_shoot();
    this->check_enemies_shoot();
    this->teleport_enemies();
    this->move_enemies();
    this->move_throwables();

    this->handle_collisions();
}

/**
 * \fn void Game::render()
 * \brief This function handles rendering various game elements like background, player, enemies and throwables on the game window.
 *
 * \return No return value (void function).
 */
void Game::render() {
    this->window_->clear(sf::Color(110,66,26));
    this->window_->draw(this->background_sprite_);
    this->player_->render(*this->window_, this->background_movement_tracker);
    this->render_enemies();
    this->render_throwables();
    this->window_->display();
}

/**
 * \fn void Game::run()
 * \brief This function contains the main loop where update and render functions are called continuously while the window is open.
 *
 * \return No return value (void function).
 */
void Game::run() {
    while(this->window_->isOpen()) {
        this->update();
        this->render();
    }
}

/**
 * \fn void Game::_init_textures()
 * \brief This function initializes various textures used in the game including player, professor and bullet textures.
 * 
 * \return No return value (void function), but exits early if any texture fails to load.
 */
void Game::_init_textures () {
    // Initialise Player Bullet Texture
    sf::Texture bullet_texture;
    if(!bullet_texture.loadFromFile("resources/Bullet.png")) return;
    this->textures.push_back(bullet_texture);
    // Initialise Player-sheet Texture
    sf::Texture player_sheet_texture;
    if(!player_sheet_texture.loadFromFile("resources/Player_Spritesheet.png")) return;
    this->textures.push_back(player_sheet_texture);
    // Initialise Professor-sheet Texture
    sf::Texture professor_sheet_texture;
    if(!professor_sheet_texture.loadFromFile("resources/Evil_Professor_Spritesheet.png")) return;
    this->textures.push_back(professor_sheet_texture);
    // Initialise Professor-Assignment Texture
    sf::Texture professor_assignment_texture;
    if(!professor_assignment_texture.loadFromFile("resources/ProfessorBullet.png")) return;
    this->textures.push_back(professor_assignment_texture);
}

/**
 * \fn void Game::_init_background()
 * \brief This function initializes the background texture and sets up its sprite with appropriate properties.
 * 
 * \return No return value (void function), but exits early if the background texture fails to load.
 */
void Game::_init_background() {
    if(!(this->background_texture_.loadFromFile("resources/background_all.png"))) return;
    this->background_sprite_.setTexture(this->background_texture_);
    this->background_sprite_.setPosition(-2.f*this->game_width_, 0.f);
    this->background_sprite_.setScale(this->x_scale_, this->y_scale_);
}

/**
 * \fn void Game::_init_player()
 * \brief This function initializes the player object with the appropriate texture.
 *
 * \return No return value (void function).
 */
void Game::_init_player() {
    this->player_ = std::make_unique<Player>(this->textures[static_cast<int>(Textures::PLAYER_SHEET)]);
}


/**
 * \fn void Game::_init_professor()
 * \brief This function initializes a new professor enemy and adds it to the enemies list.
 *
 * \return No return value (void function).
 */
void Game::_init_professor() {
    this->enemies_.push_back(std::make_unique<Professor>(this->textures[static_cast<int>(Textures::PROFESSOR_SHEET)]));
}

/**
 * \fn void Game::teleport_enemies()
 * \brief This function handles the logic for spawning new professor enemies at regular intervals until a maximum limit is reached.
 *
 * \return No return value (void function).
 */
void Game::teleport_enemies () {
    if (Professor::get_num_professors() < 300) {
        if(this->professor_cool_down > this->max_professor_cool_down) {
            this->_init_professor();
            this->professor_cool_down = 0;
        }
        else {
            this->professor_cool_down++;
        }
    }
}

/**
 * \fn void Game::move_enemies()
 * \brief This function handles the movement and status updates for each enemy in the game. It also bins each enemy into the correct vacinity used for collision detection
 *
 * \return No return value (void function).
 */
void Game::move_enemies () {
    std::vector<std::shared_ptr<Enemy>> actual_empty_bin;
    this->actual_enemy_vicinities_ = std::vector<std::vector<std::shared_ptr<Enemy>>>(1000, actual_empty_bin);
    for (auto i = 0; i < this->enemies_.size(); i++) {
        if (!this->enemies_[i]->is_dying()) {
            this->enemies_[i]->move(this->background_movement_tracker, this->background_location_, sf::Vector2f(this->player_->get_position().x_left, this->player_->get_position().y));
            this->bin_vicinities(this->get_vicinities(this->enemies_[i]->get_world_bounds(), i), this->enemies_[i]);
        } else if (!this->enemies_[i]->get_is_dead()) {
            this->enemies_[i]->destroy();
        } else {
            this->erase_enemy(i);
        }
    }
}

/**
 * \fn void Game::bin_vicinities(std::vector<int> vicinities, int position)
 * \brief This function updates the enemy vicinities based on the given position and vicinity indices.
 *
 * \param vicinities A vector containing the indices of the vicinities.
 * \param enemy pointer to an Enemy object
 * 
 * \return No return value (void function).
 */
void Game::bin_vicinities (std::vector<int> vicinities, std::shared_ptr<Enemy> enemy) {
    for (auto actual_vicinity_index : vicinities) {
        // std::cout << "vicinity index = " << actual_vicinity_index << std::endl;
        // if(actual_vicinity_index > 498 || actual_vicinity_index < 0) {
        //     std::cout << "vicinity index = " << actual_vicinity_index << std::endl;
        // }
        this->actual_enemy_vicinities_[actual_vicinity_index].push_back(enemy);
    }
}

/**
 * \fn std::vector<int> Game::get_vicinities(sf::FloatRect rect, int position)
 * \brief This function calculates the vicinity indices based on a given rectangular area and position.
 *
 * \param rect A float rectangle that defines the area.
 * \param position The position index.
 * 
 * \return A vector containing the unique vicinity indices.
 */
std::vector<int> Game::get_vicinities (sf::FloatRect rect, int position) {
    std::vector<int> vicinities;
    int left_location = floor(rect.left/100)+28;
    int right_location = floor((rect.left + rect.width)/100)+28;
    int top_location = floor((rect.top)/100);
    int bottom_location = floor((rect.top+rect.height)/100);
    vicinities.push_back(top_location*70 + 1 + left_location);
    vicinities.push_back(top_location*70 + 1 + right_location);
    vicinities.push_back(bottom_location*70 + 1 + left_location);
    vicinities.push_back(bottom_location*70 + 1 + right_location);

    if(vicinities[0] < 0) {
        std::cout << "rect.left = " << rect.left << "\trect.top = " << rect.top << std::endl;
     }

    sort(vicinities.begin(), vicinities.end());
    auto new_end = unique(vicinities.begin(), vicinities.end());
    vicinities.erase(new_end, vicinities.end());

    return vicinities;
}

/**
 * \fn void Game::render_enemies()
 * \brief This function iterates over all enemies and triggers their render function.
 *
 * \return No return value (void function).
 */
void Game::render_enemies () {
    for (auto i = 0; i < enemies_.size(); i++) this->enemies_[i]->render(*this->window_, this->background_movement_tracker);
}

/**
 * \fn void Game::move_throwables()
 * \brief This function handles the movement of throwables in the game, removing them if they move out of the allowable area.
 *
 * \return No return value (void function).
 */
void Game::move_throwables () {
    for (auto i = 0; i < this->throwables_.size(); i++) {
        auto in_horizontal = this->throwables_[i]->get_location().x > -700.f && this->throwables_[i]->get_location().x < 2100.f;
        auto in_vertical = this->throwables_[i]->get_location().y > -30.f && this->throwables_[i]->get_location().y < 830.f;
        if(in_horizontal && in_vertical) {
            this->throwables_[i]->move(this->background_movement_tracker);
        } else this->erase_throwable(i);
    }
}

/**
 * \fn void Game::render_throwables()
 * \brief This function iterates over all throwables and triggers their draw function.
 *
 * \return No return value (void function).
 */
void Game::render_throwables () {
    for (auto i = 0; i < this->throwables_.size(); i++) {
            this->throwables_[i]->draw(*this->window_);
    }
}

/**
 * \fn void Game::handle_collisions()
 * \brief This function is a wrapper for various collision checking functions.
 *
 * \return No return value (void function).
 */
void Game::handle_collisions () {
    this->check_bullet_enemy_collision();
}

/**
 * \fn void Game::check_bullet_enemy_collision()
 * \brief This function checks for collisions between bullets and enemies, destroying any entities involved in a collision.
 *
 * \return No return value (void function).
 */
void Game::check_bullet_enemy_collision () {
    for (auto bullet : this->player_->get_bullets()) {
        bool bullet_hit = false;
        std::vector<int> bullet_vicinities = this->get_vicinities(bullet->get_world_bounds(), -1);
        for (auto vicinity : bullet_vicinities) {
            for (auto enemy : this->actual_enemy_vicinities_[vicinity]) {
                bool hit = enemy->get_world_bounds().intersects(bullet->get_world_bounds());
                if (hit) {
                    enemy->destroy();
                    this->player_->erase_bullet(bullet);
                    bullet_hit = true;
                }
                if (bullet_hit) break;
            }
            if (bullet_hit) break;
        }
    }
}

/**
 * \fn void Game::erase_enemy(int position)
 * \brief This function removes an enemy from the enemies list at the specified position.
 *
 * \param position The index of the enemy to be removed.
 * \return No return value (void function).
 */
void Game::erase_enemy(int position) {
    this->enemies_.erase(this->enemies_.begin() + position);
}

/** 
 * \fn void Game::erase_throwable(int position)
 * \brief Erases a throwable object from the list of throwables at the specified position
 *
 * \param position The position of the throwable object to erase
 * \return void
 */
void Game::erase_throwable (int position) {
    this->throwables_.erase(this->throwables_.begin() + position);
}

/** 
 * \fn void Game::check_player_shoot()
 * \brief Handles the player's shooting actions based on keyboard inputs
 * 
 * \return void
 */
void Game::check_player_shoot() {
    this->player_->increment_cool_down();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !this->shot_held) {
        this->player_->shoot_bullet(this->textures[static_cast<int>(Textures::BULLET)]);
        this->shot_held = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        this->shot_held = false;
    }
}

/** 
 * \fn void Game::check_enemies_shoot()
 * \brief Checks and handles shooting actions of all enemies
 * 
 * \return void
 */
void Game::check_enemies_shoot() {
    for (auto i = 0; i < this->enemies_.size(); i++){
        this->enemies_[i]->increment_cool_down();
        auto new_throwable = this->enemies_[i]->shoot_throwable(this->textures[static_cast<int>(Textures::PROFESSOR_ASSIGNMENT)], sf::Vector2f(this->player_->get_position().x_left, this->player_->get_position().y));
        if (new_throwable != nullptr) this->throwables_.push_back(new_throwable);
    }
}

/** 
 * \fn void Game::handle_boundary_background_movement()
 * \brief Handles the background movement within specified boundaries based on background location and movement variables
 * 
 * \return void
 */
void Game::handle_boundary_background_movement () {
    float tolerance = 0.0001f;
    if (this->background_movement_ > tolerance) {
        this->background_movement_ -= background_acceleration_;
        this->background_movement_tracker = this->background_movement_;
        this->background_sprite_.move(this->background_movement_,0.f);
    }
    else if (this->background_movement_ < -tolerance) {
        this->background_movement_ += background_acceleration_;
        this->background_movement_tracker = this->background_movement_;
        this->background_sprite_.move(this->background_movement_,0.f); 
    } else {
        this->background_movement_ = 0;
        this->background_movement_tracker = this->background_movement_;
    }
    this->wrap();
}

/** 
 * \fn void Game::handle_internal_background_movement()
 * \brief Handles the internal background movement based on player's speed and keyboard inputs
 * 
 * \return void
 */
void Game::handle_internal_background_movement () {
    float player_internal_movement = this->player_->get_player_speed();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        player_internal_movement += this->player_->get_player_speed() >= 0 ? 0.4f : -0.4f;
    }
    this->background_movement_tracker = -player_internal_movement;
    this->background_sprite_.move(-player_internal_movement, 0.f);
    this->background_location_ += -player_internal_movement;
    this->wrap();
}

/** 
 * \fn void Game::internal_movement(float x_right, float x_left)
 * \brief Handles the internal movement of the player and background based on various conditions and keyboard inputs
 *
 * \param x_right The right boundary for player movement
 * \param x_left The left boundary for player movement
 * \return void
 */
void Game::internal_movement (float x_right, float x_left) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        if (x_left > this->player_->get_x_default_left()) {
            this->player_->move_player_horizontal(Direction::LEFT);
            this->handle_internal_background_movement();
            this->background_movement_ = 0.f;
        } else {
            this->background_sprite_.move(-this->background_base_speed_, 0.f);
            this->background_movement_ = -this->background_base_speed_;
            this->background_movement_tracker = this->background_movement_;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        if(x_right < this->player_->get_x_default_right()) {
            this->player_->move_player_horizontal(Direction::RIGHT);
            this->handle_internal_background_movement();
            this->background_movement_ = 0.f;
        } else {
            this->background_sprite_.move(this->background_base_speed_, 0.f);
            this->background_movement_ = this->background_base_speed_;
            this->background_movement_tracker = this->background_movement_;
        }
    }

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
        this->player_->magnetise_player();
        if ((x_right < this->player_->get_x_default_right() && this->player_->get_player_speed() < 0) || 
            (x_left > this->player_->get_x_default_left() && this->player_->get_player_speed() > 0)) {
                this->handle_internal_background_movement();
            } else this->handle_boundary_background_movement();
    }
    this->background_location_ += this->background_movement_;
    this->wrap();
}

/** 
 * \fn void Game::wrap()
 * \brief Controls background movement to wrap back around creating infinite scrolling
 * 
 * \return void
 */
void Game::wrap() {
    auto left_bounds = 2600.f;
    auto right_bounds = -4350.f;
    auto correction =left_bounds-right_bounds;
    if (this->background_location_ >= left_bounds) {
        this->background_location_ += -correction;
        this->background_sprite_.move(-correction, 0.f);
    }
    if (this->background_location_ <= right_bounds) {
        this->background_location_ += correction;
        this->background_sprite_.move(correction, 0.f);
    }
}

/** 
 * \fn void Game::move_player()
 * \brief Controls the player's movement by considering edge cases, background location, and keyboard inputs
 * 
 * \return void
 */
void Game::move_player() {
    float x_right = this->player_->get_position().x_right;
    float x_left = this->player_->get_position().x_left;

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        if (this->player_->get_prev_vertical_direction() == Direction::UP) this->player_->move_player_vertical(Direction::UP, false);
        if (this->player_->get_prev_vertical_direction() == Direction::DOWN) this->player_->move_player_vertical(Direction::DOWN, false);
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player_vertical(Direction::UP, true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player_vertical(Direction::DOWN, true);
    }
    this->internal_movement(x_right, x_left);
}