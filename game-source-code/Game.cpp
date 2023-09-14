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
    _init_background();
    _init_textures();
    _init_player();
}

void Game::update() {
    sf::Event event;
    while (window_->pollEvent(event)) if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) window_->close();
    // Handle Pause
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !paused_pressed_) {
        paused_ = !paused_;
        paused_pressed_ = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) paused_pressed_ = false;
    if (paused_) return;

    move_player();
    player_->communicate_position(background_location_);
    check_player_shoot();
    check_enemies_shoot();
    teleport_enemies(7);
    move_enemies();
    move_throwables();

    handle_collisions();
}

void Game::render() {
    window_->clear(sf::Color(110,66,26));
    window_->draw(background_sprite_);
    player_->render(*window_, background_movement_tracker);
    render_enemies();
    render_throwables();
    window_->display();
}

void Game::run() {
    while(window_->isOpen()) {
        update();
        render();
    }
}

void Game::_init_textures () {
    // Initialise Player Bullet Texture
    sf::Texture bullet_texture;
    if(!bullet_texture.loadFromFile("resources/Bullet.png")) return;
    textures.push_back(bullet_texture);
    // Initialise Player-sheet Texture
    sf::Texture player_sheet_texture;
    if(!player_sheet_texture.loadFromFile("resources/Player_Spritesheet.png")) return;
    textures.push_back(player_sheet_texture);
    // Initialise Professor-sheet Texture
    sf::Texture professor_sheet_texture;
    if(!professor_sheet_texture.loadFromFile("resources/Evil_Professor_Spritesheet.png")) return;
    textures.push_back(professor_sheet_texture);
    // Initialise Professor-Assignment Texture
    sf::Texture professor_assignment_texture;
    if(!professor_assignment_texture.loadFromFile("resources/ProfessorBullet.png")) return;
    textures.push_back(professor_assignment_texture);
}

void Game::_init_background() {
    if(!(background_texture_.loadFromFile("resources/background_all.png"))) return;
    background_sprite_.setTexture(background_texture_);
    background_sprite_.setPosition(-2.f*game_width_, 0.f);
    background_sprite_.setScale(x_scale_, y_scale_);
}

void Game::_init_player() {
    player_ = std::make_unique<Player>(textures[static_cast<int>(Textures::PLAYER_SHEET)]);
}

void Game::_init_professor() {
    enemies_.push_back(std::make_unique<Professor>(textures[static_cast<int>(Textures::PROFESSOR_SHEET)]));
}

void Game::teleport_enemies (int num_enemies) {
    if (Professor::get_num_professors() < num_enemies) {
        if(professor_cool_down > max_professor_cool_down) {
            _init_professor();
            professor_cool_down = 0;
        }
        else {
            professor_cool_down++;
        }
    }
}

void Game::move_enemies () {
    std::vector<std::shared_ptr<Enemy>> actual_empty_bin;
    actual_enemy_vicinities_ = std::vector<std::vector<std::shared_ptr<Enemy>>>(576, actual_empty_bin);
    for (auto i = 0; i < enemies_.size(); i++) {
        if (!enemies_[i]->is_dying()) {
            enemies_[i]->move(background_movement_tracker, background_location_, sf::Vector2f(player_->get_position().x_left, player_->get_position().y));
            bin_vicinities(get_vicinities(enemies_[i]->get_world_bounds(), i), enemies_[i]);
        } else if (!enemies_[i]->get_is_dead()) {
            enemies_[i]->destroy();
        } else {
            erase_enemy(i);
        }
    }
}

void Game::bin_vicinities (std::vector<int> vicinities, std::shared_ptr<Enemy> enemy) {
    for (auto actual_vicinity_index : vicinities) {
        actual_enemy_vicinities_[actual_vicinity_index].push_back(enemy);
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
    for (auto i = 0; i < enemies_.size(); i++) enemies_[i]->render(*window_, background_movement_tracker);
}

void Game::move_throwables () {
    for (auto i = 0; i < throwables_.size(); i++) {
        auto in_horizontal = throwables_[i]->get_location().x > -700.f && throwables_[i]->get_location().x < 2100.f;
        auto in_vertical = throwables_[i]->get_location().y > -30.f && throwables_[i]->get_location().y < 830.f;
        if(in_horizontal && in_vertical) {
            throwables_[i]->move(background_movement_tracker);
        } else erase_throwable(i);
    }
}

void Game::render_throwables () {
    for (auto i = 0; i < throwables_.size(); i++) {
            throwables_[i]->draw(*window_);
    }
}
// serves as a wrapper function for all collisions to be added later
void Game::handle_collisions () {
    check_bullet_enemy_collision();
}

//--------------------------------------------------------------------------------
// In order to avoid O(n^2) collision detection, every game object is dynamically
// mapped to a vicinity. Therefore, when checking collisions, only objects within
// the vicinity are considered. This structure creates an effevtively O(n)
// collision detection schedule.
//--------------------------------------------------------------------------------
void Game::check_bullet_enemy_collision () {
    for (auto bullet : player_->get_bullets()) {
        bool bullet_hit = false;
        std::vector<int> bullet_vicinities = get_vicinities(bullet->get_world_bounds(), -1);
        for (auto vicinity : bullet_vicinities) {
            for (auto enemy : actual_enemy_vicinities_[vicinity]) {
                bool hit = enemy->get_world_bounds().intersects(bullet->get_world_bounds());
                if (hit) {
                    enemy->destroy();
                    player_->erase_bullet(bullet);
                    bullet_hit = true;
                }
                if (bullet_hit) break;
            }
            if (bullet_hit) break;
        }
    }
}

void Game::erase_enemy(int position) {
    enemies_.erase(enemies_.begin() + position);
}

void Game::erase_throwable (int position) {
    throwables_.erase(throwables_.begin() + position);
}

void Game::check_player_shoot() {
    player_->increment_cool_down();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !shot_held) {
        player_->shoot_bullet(textures[static_cast<int>(Textures::BULLET)]);
        shot_held = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        shot_held = false;
    }
}

void Game::check_enemies_shoot() {
    for (auto i = 0; i < enemies_.size(); i++){
        enemies_[i]->increment_cool_down();
        auto new_throwable = enemies_[i]->shoot_throwable(textures[static_cast<int>(Textures::PROFESSOR_ASSIGNMENT)], sf::Vector2f(player_->get_position().x_left, player_->get_position().y));
        if (new_throwable != nullptr) throwables_.push_back(new_throwable);
    }
}

void Game::handle_boundary_background_movement () {
    float tolerance = 0.0001f;
    if (background_movement_ > tolerance) {
        background_movement_ -= background_acceleration_;
        background_movement_tracker = background_movement_;
        background_sprite_.move(background_movement_,0.f);
    }
    else if (background_movement_ < -tolerance) {
        background_movement_ += background_acceleration_;
        background_movement_tracker = background_movement_;
        background_sprite_.move(background_movement_,0.f); 
    } else {
        background_movement_ = 0;
        background_movement_tracker = background_movement_;
    }
    wrap();
}

void Game::handle_internal_background_movement () {
    float player_internal_movement = player_->get_player_speed();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        player_internal_movement += player_->get_player_speed() >= 0 ? 0.4f : -0.4f;
    }
    background_movement_tracker = -player_internal_movement;
    background_sprite_.move(-player_internal_movement, 0.f);
    background_location_ += -player_internal_movement;
    wrap();
}

void Game::internal_movement (float x_right, float x_left) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        if (x_left > player_->get_x_default_left()) {
            player_->move_player_horizontal(Direction::LEFT);
            handle_internal_background_movement();
            background_movement_ = 0.f;
        } else {
            background_sprite_.move(-background_base_speed_, 0.f);
            background_movement_ = -background_base_speed_;
            background_movement_tracker = background_movement_;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        if(x_right < player_->get_x_default_right()) {
            player_->move_player_horizontal(Direction::RIGHT);
            handle_internal_background_movement();
            background_movement_ = 0.f;
        } else {
            background_sprite_.move(background_base_speed_, 0.f);
            background_movement_ = background_base_speed_;
            background_movement_tracker = background_movement_;
        }
    }

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
        player_->magnetise_player();
        if ((x_right < player_->get_x_default_right() && player_->get_player_speed() < 0) || 
            (x_left > player_->get_x_default_left() && player_->get_player_speed() > 0)) {
                handle_internal_background_movement();
            } else handle_boundary_background_movement();
    }
    background_location_ += background_movement_;
    wrap();
}

//--------------------------------------------------------------------------------
// In order to create an infinite scrolling screen for gameplay, the background
// is updated upon rwaching the bounds of the world.
//--------------------------------------------------------------------------------
void Game::wrap() {
    auto left_bounds = 2600.f;
    auto right_bounds = -4350.f;
    auto correction =left_bounds-right_bounds;
    if (background_location_ >= left_bounds) {
        background_location_ += -correction;
        background_sprite_.move(-correction, 0.f);
    }
    if (background_location_ <= right_bounds) {
        background_location_ += correction;
        background_sprite_.move(correction, 0.f);
    }
}

void Game::move_player() {
    float x_right = player_->get_position().x_right;
    float x_left = player_->get_position().x_left;

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        if (player_->get_prev_vertical_direction() == Direction::UP) player_->move_player_vertical(Direction::UP, false);
        if (player_->get_prev_vertical_direction() == Direction::DOWN) player_->move_player_vertical(Direction::DOWN, false);
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) player_->move_player_vertical(Direction::UP, true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) player_->move_player_vertical(Direction::DOWN, true);
    }
    internal_movement(x_right, x_left);
}

 std::vector<std::shared_ptr<Enemy>> Game::get_enemies() { return enemies_; }
  std::vector<std::shared_ptr<Throwable>> Game::get_throwables() { return throwables_; }