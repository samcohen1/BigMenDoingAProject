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

Game::Game(std::shared_ptr<sf::RenderWindow> window, float original_background_width, float original_background_height, float game_width, float game_height, float x_scale, float y_scale) : 
        window_(window), original_background_width_(original_background_width), original_background_height_(original_background_height), game_width_(game_width), game_height_(game_height), x_scale_(x_scale), y_scale_(y_scale) {
    this->_init_background();
    this->_init_textures();
    this->_init_player();
}

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
    this->teleport_enemies(50);
    this->move_enemies();
    this->move_throwables();

    this->handle_collisions();
}

void Game::render() {
    this->window_->clear(sf::Color(110,66,26));
    this->window_->draw(this->background_sprite_);
    this->player_->render(*this->window_, this->background_movement_tracker);
    this->render_enemies();
    this->render_throwables();
    this->window_->display();
}

void Game::run() {
    while(this->window_->isOpen()) {
        this->update();
        this->render();
    }
}

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

void Game::_init_background() {
    if(!(this->background_texture_.loadFromFile("resources/background_all.png"))) return;
    this->background_sprite_.setTexture(this->background_texture_);
    this->background_sprite_.setPosition(-2.f*this->game_width_, 0.f);
    this->background_sprite_.setScale(this->x_scale_, this->y_scale_);
}

void Game::_init_player() {
    this->player_ = std::make_unique<Player>(this->textures[static_cast<int>(Textures::PLAYER_SHEET)]);
}

void Game::_init_professor() {
    this->enemies_.push_back(std::make_unique<Professor>(this->textures[static_cast<int>(Textures::PROFESSOR_SHEET)]));
}

void Game::teleport_enemies (int num_enemies) {
    if (Professor::get_num_professors() < num_enemies) {
        if(this->professor_cool_down > this->max_professor_cool_down) {
            this->_init_professor();
            this->professor_cool_down = 0;
        }
        else {
            this->professor_cool_down++;
        }
    }
}

void Game::move_enemies () {
    std::vector<std::shared_ptr<Enemy>> actual_empty_bin;
    this->actual_enemy_vicinities_ = std::vector<std::vector<std::shared_ptr<Enemy>>>(576, actual_empty_bin);
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

void Game::bin_vicinities (std::vector<int> vicinities, std::shared_ptr<Enemy> enemy) {
    for (auto actual_vicinity_index : vicinities) {
        this->actual_enemy_vicinities_[actual_vicinity_index].push_back(enemy);
    }
}

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

    sort(vicinities.begin(), vicinities.end());
    auto new_end = unique(vicinities.begin(), vicinities.end());
    vicinities.erase(new_end, vicinities.end());

    return vicinities;
}

void Game::render_enemies () {
    for (auto i = 0; i < enemies_.size(); i++) this->enemies_[i]->render(*this->window_, this->background_movement_tracker);
}

void Game::move_throwables () {
    for (auto i = 0; i < this->throwables_.size(); i++) {
        auto in_horizontal = this->throwables_[i]->get_location().x > -700.f && this->throwables_[i]->get_location().x < 2100.f;
        auto in_vertical = this->throwables_[i]->get_location().y > -30.f && this->throwables_[i]->get_location().y < 830.f;
        if(in_horizontal && in_vertical) {
            this->throwables_[i]->move(this->background_movement_tracker);
        } else this->erase_throwable(i);
    }
}

void Game::render_throwables () {
    for (auto i = 0; i < this->throwables_.size(); i++) {
            this->throwables_[i]->draw(*this->window_);
    }
}
// serves as a wrapper function for all collisions to be added later
void Game::handle_collisions () {
    this->check_bullet_enemy_collision();
}

//--------------------------------------------------------------------------------
// In order to avoid O(n^2) collision detection, every game object is dynamically
// mapped to a vicinity. Therefore, when checking collisions, only objects within
// the vicinity are considered. This structure creates an effevtively O(n)
// collision detection schedule.
//--------------------------------------------------------------------------------
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

void Game::erase_enemy(int position) {
    this->enemies_.erase(this->enemies_.begin() + position);
}

void Game::erase_throwable (int position) {
    this->throwables_.erase(this->throwables_.begin() + position);
}

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

void Game::check_enemies_shoot() {
    for (auto i = 0; i < this->enemies_.size(); i++){
        this->enemies_[i]->increment_cool_down();
        auto new_throwable = this->enemies_[i]->shoot_throwable(this->textures[static_cast<int>(Textures::PROFESSOR_ASSIGNMENT)], sf::Vector2f(this->player_->get_position().x_left, this->player_->get_position().y));
        if (new_throwable != nullptr) this->throwables_.push_back(new_throwable);
    }
}

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

//--------------------------------------------------------------------------------
// In order to create an infinite scrolling screen for gameplay, the background
// is updated upon rwaching the bounds of the world.
//--------------------------------------------------------------------------------
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

 std::vector<std::shared_ptr<Enemy>> Game::get_enemies() { return this->enemies_; }
  std::vector<std::shared_ptr<Throwable>> Game::get_throwables() { return this->throwables_; }