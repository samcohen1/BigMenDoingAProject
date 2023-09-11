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
    // this->_init_window();
    this->_init_background();
    this->_init_textures();
    // this->_init_home_screen();
    // this->_init_about_screen();
    this->_init_player();
    this->_init_professor();
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
    this->move_throwables();
    this->player_->communicate_position(this->background_location_);
    this->check_player_shoot();
    this->check_enemies_shoot();
    this->move_enemies();
    this->teleport_enemies();

    this->handle_collisions();
}

void Game::render() {
    this->window_->clear(sf::Color(110,66,26));
    this->window_->draw(this->background_sprite_);
    this->player_->render(*this->window_, this->background_movement_);
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

// void Game::_init_window() {
//     this->window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(this->game_width_, this->game_height_, 32), "Byte Defenders", sf::Style::Titlebar | sf::Style::Close);
// }

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
    if(!(this->background_texture_.loadFromFile("resources/Background.png"))) return;
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

void Game::teleport_enemies () {
    if (Professor::get_num_professors() < 5) {
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
    this->enemy_vicinities_ = std::vector<std::vector<int>>(490);
    for (auto i = 0; i < this->enemies_.size(); i++) {
        if (!this->enemies_[i]->is_dying()) {
            this->enemies_[i]->move(this->background_location_, sf::Vector2f(this->player_->get_position().x_left, this->player_->get_position().y));
            this->bin_vicinities(this->get_vicinities(this->enemies_[i]->get_world_bounds(), i), i);
        } else if (!this->enemies_[i]->get_is_dead()) {
            this->enemies_[i]->destroy();
        } else {
            this->erase_enemy(i);
        }
    }
}

void Game::bin_vicinities (std::vector<int> vicinities, int position) {
    for (auto vicinity_index : vicinities) enemy_vicinities_[vicinity_index].push_back(position);
}

std::vector<int> Game::get_vicinities (sf::FloatRect rect, int position) {
    std::vector<int> vicinities;
    int left_location = floor(rect.left/100)+28;
    int right_location = floor((rect.left + rect.width)/100)+28;
    int top_location = floor((rect.top)/100)-1;
    int bottom_location = floor((rect.top+rect.height)/100)-1;
    vicinities.push_back(left_location*7 + top_location);
    vicinities.push_back(right_location*7 + top_location);
    vicinities.push_back(left_location*7 + bottom_location);
    vicinities.push_back(right_location*7 + bottom_location);

    sort(vicinities.begin(), vicinities.end());
    auto new_end = unique(vicinities.begin(), vicinities.end());
    vicinities.erase(new_end, vicinities.end());

    return vicinities;
}

void Game::render_enemies () {
    for (auto i = 0; i < enemies_.size(); i++) {
        this->enemies_[i]->render(*this->window_, this->background_movement_tracker);
    }
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

void Game::handle_collisions () {
    this->check_bullet_enemy_collision();
}

void Game::check_bullet_enemy_collision () {
    for (auto bullet_index = 0; bullet_index < this->player_->get_bullets().size(); bullet_index++) {
        bool bullet_hit = false;
        std::vector<int> bullet_vicinities = this->get_vicinities(this->player_->get_bullets()[bullet_index]->get_world_bounds(), -1);
        for (auto bullet_vicinity_index = 0; bullet_vicinity_index < bullet_vicinities.size(); bullet_vicinity_index++) {
            for (auto enemy_index = 0; enemy_index < this->enemy_vicinities_[bullet_vicinities[bullet_vicinity_index]].size(); enemy_index++) {
                bool hit = this->enemies_[enemy_vicinities_[bullet_vicinities[bullet_vicinity_index]][enemy_index]]->get_bounds().intersects(this->player_->get_bullets()[bullet_index]->get_bounds());
                if (hit) {
                    this->enemies_[enemy_vicinities_[bullet_vicinities[bullet_vicinity_index]][enemy_index]]->destroy();
                    this->player_->erase_bullet(bullet_index);
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

bool Game::approx_equal (float a, float b) {
    float tolerance = 0.01f;
    return abs(a-b) <= tolerance;
}

bool Game::approx_innequality (float a, float b, bool greater_than) {
    float tolerance = 0.01;
    if (greater_than) return a > b+tolerance;
    return a < b+tolerance;
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
    if (this->background_location_<2*this->game_width_ && this->background_location_>-2*this->game_width_) {
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
    } else {
        this->background_movement_ = 0;
        this->background_movement_tracker = this->background_movement_;
    }
}

void Game::handle_internal_background_movement () {
    float player_internal_movement = this->player_->get_player_speed();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        player_internal_movement += this->player_->get_player_speed() >= 0 ? 0.4f : -0.4f;
    }
    this->background_movement_tracker = -player_internal_movement;
    this->background_sprite_.move(-player_internal_movement, 0.f);
    this->background_location_ += -player_internal_movement;
}

void Game::internal_movement (float x_right, float x_left) {
    if (this->prev_in_edge) this->player_->correct_edge_positions();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        if (x_left > this->player_->get_x_default_left()) {
            this->player_->move_player_horizontal(Direction::LEFT);
            this->handle_internal_background_movement();
            this->background_movement_ = 0.f;
        }
        else if (this->background_location_ > -2*this->game_width_) {
            this->background_sprite_.move(-this->background_base_speed_, 0.f);
            this->background_movement_ = -this->background_base_speed_;
            this->background_movement_tracker = this->background_movement_;
        } else {
            this->background_movement_ = 0;
            this->background_movement_tracker = this->background_movement_;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        if(x_right < this->player_->get_x_default_right()) {
            this->player_->move_player_horizontal(Direction::RIGHT);
            this->handle_internal_background_movement();
            this->background_movement_ = 0.f;
        }
        else if (this->background_location_ < 2*this->game_width_)  {
            this->background_sprite_.move(this->background_base_speed_, 0.f);
            this->background_movement_ = this->background_base_speed_;
            this->background_movement_tracker = this->background_movement_;
        } else {
            this->background_movement_ = 0;
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
    this->prev_in_edge = false;
}

void Game::edge_movement(float x_right, float x_left) {
    bool sideKey = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && this->player_->get_position().x_right <= 1377.f) {
        sideKey = true;
        this->player_->edge_player_movement(Direction::LEFT);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && this->player_->get_position().x_left >= 23.f) {
        sideKey = true;
        this->player_->edge_player_movement(Direction::RIGHT);
    }
    if(!sideKey && this->player_->get_position().x_left >= 23.f && this->player_->get_position().x_right <= 1377.f) {
        this->player_->edge_decelerate();
    }
    this->prev_in_edge = true;
}

void Game::move_player() {
    float x_right = this->player_->get_position().x_right;
    float x_left = this->player_->get_position().x_left;
    float edge_tolerance = 2.f;
    bool in_edge_case = (this->background_location_ > 2*this->game_width_-edge_tolerance && x_left <= this->player_->get_x_default_right()+edge_tolerance) || 
                        (this->background_location_ < -2*this->game_width_ && x_right >= this->player_->get_x_default_left()-edge_tolerance);

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        if (this->player_->get_prev_vertical_direction() == Direction::UP) this->player_->move_player_vertical(Direction::UP, false);
        if (this->player_->get_prev_vertical_direction() == Direction::DOWN) this->player_->move_player_vertical(Direction::DOWN, false);
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) this->player_->move_player_vertical(Direction::UP, true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) this->player_->move_player_vertical(Direction::DOWN, true);
    }

    if (in_edge_case) {
        this->background_movement_tracker = 0.f;
        this->edge_movement(x_right, x_left);
        return;
    } else this->internal_movement(x_right, x_left);
}


// SPLASH
// void Game::update_home() {
//     sf::Event event;
//     this->mouse_over = false;

//     while (this->window_->pollEvent(event)) {
//         if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
//             this->window_->close();
//         }
//     }
//     if(home_option_selected < 2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !held_down) {
//        this->shift_option_down();
//     }
//     if(home_option_selected > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !held_down) {
//        this->shift_option_up();
//     }
//     if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
//         this->held_down=false;
//     }
//     this->select_using_mouse();
//     if((mouse_over && sf::Mouse::isButtonPressed(sf::Mouse::Left))  || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
//         if(this->home_option_selected == 0) this->is_playing = true;
//         if(this->home_option_selected == 1) {
//             this->about_screen_showing = true;
//             this->option_is_selected = true;
//         }
//         if(this->home_option_selected == 2); //Do how to play
//     }
// }

// void Game::update_about() {
//     sf::Event event;
//     while (this->window_->pollEvent(event)) {
//         if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
//             this->window_->close();
//         }
//     }
// }

// void Game::render_home() {
//     this->window_->clear();
//     this->window_->draw(this->background_sprite_);
//     for (auto i = 0; i<this->home_options.size(); i++) {
//         this->window_->draw(this->home_sprites[i]);
//         this->window_->draw(this->home_options[i]);
//     }
//     this->window_->display();
// }

// void Game::render_about() {
//     this->window_->clear();
//     this->window_->draw(this->background_sprite_);
//     this->window_->draw(this->message_screen_sprite);
//     this->window_->draw(this->messages_[0]);
//     this->window_->draw(this->messages_[1]);
//     this->window_->display();
// }

// void Game::_init_home_screen() {
//     this->home_sprites = std::vector<sf::Sprite>(3);
//     this->home_options = std::vector<sf::Text>(3);
//     if(!this->options_texture.loadFromFile("resources/home_options_spritesheet.png")) return;
//     if(!this->font.loadFromFile("resources/pixel_text.ttf")) return;
//     float x_position = (this->window_->getSize().x/2.f) - 120.f;
//     this->home_options[0].setFont(this->font);
//     this->home_options[0].setFillColor(sf::Color::White);
//     this->home_options[0].setCharacterSize(18.f);
//     this->home_options[0].setString("Play");
//     this->home_sprites[0].setTexture(this->options_texture);
//     this->home_sprites[0].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
//     this->home_sprites[0].setScale(0.5f, 0.5f);
//     float y_position_text = this->home_sprites[0].getGlobalBounds().height/2 - this->home_options[0].getGlobalBounds().height;
//     float x_position_text = this->home_sprites[0].getGlobalBounds().width/2 - this->home_options[0].getGlobalBounds().width/2;
//     this->home_sprites[0].setPosition(x_position, 100.f);
//     this->home_options[0].setPosition(x_position + x_position_text, 105.f + y_position_text);
//     this->home_sprites[1].setTexture(this->options_texture);
//     this->home_sprites[1].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//     this->home_sprites[1].setPosition(x_position, 300.f);
//     this->home_sprites[1].setScale(0.5f, 0.5f);
//     this->home_options[1].setFont(this->font);
//     this->home_options[1].setFillColor(sf::Color::White);
//     this->home_options[1].setString("Backstory");
//     this->home_options[1].setCharacterSize(18.f);
//     x_position_text = this->home_sprites[1].getGlobalBounds().width/2 - this->home_options[1].getGlobalBounds().width/2;
//     this->home_options[1].setPosition(x_position + x_position_text, 305.f + y_position_text);
//     this->home_sprites[2].setTexture(this->options_texture);
//     this->home_sprites[2].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//     this->home_sprites[2].setPosition(x_position, 500.f);
//     this->home_sprites[2].setScale(0.5f, 0.5f);
//     this->home_options[2].setFont(this->font);
//     this->home_options[2].setFillColor(sf::Color::White);
//     this->home_options[2].setString("How to Play");
//     this->home_options[2].setCharacterSize(18.f);
//     x_position_text = this->home_sprites[2].getGlobalBounds().width/2 - this->home_options[2].getGlobalBounds().width/2;
//     this->home_options[2].setPosition(x_position + x_position_text, 505.f + y_position_text);
// }

// void Game::_init_about_screen() {
//     this->messages_ = std::vector<sf::Text>(3);
//     this->messages_[0].setFont(this->font);
//     this->messages_[0].setFillColor(sf::Color::White);
//     this->messages_[0].setString("About Graduation-Hat Hackers");
//     this->messages_[0].setPosition(this->window_->getSize().x/2 - this->messages_[0].getGlobalBounds().width/2, 100.f);
//     this->messages_[0].setCharacterSize(16.f);
//     this->messages_[1].setFont(this->font);
//     this->messages_[1].setFillColor(sf::Color::White);
//     this->messages_[1].setCharacterSize(16.f);
//     std::string about_message = "For a Wits University student the campus is a daunting place.\n";
//     about_message += "The professors constantly attack students with assignments\nand the scientists are dangerous to be around in general.\n";
//     about_message +="And we have not even mentioned the happenings on East Campus!\n";
//     about_message +="But one brave group have found a way to survive through\nthis tough existence: The software developers.\n";
//     about_message +="Your job is to become a software developer.\n";
//     about_message +="You must shoot code at these treacherous enemies\nand protect the future of the university,\nwhile simultaneously avoiding becoming caught up\n";
//     about_message +="by the bombs and assignments of the evil faculties.";
//     this->messages_[1].setString(about_message);
//     if(!this->message_screen_texture.loadFromFile("resources/splash_background.png")) return;
//     this->message_screen_sprite.setTexture(this->message_screen_texture);
//     this->message_screen_sprite.setScale(1.3f, 0.7f);
//     this->message_screen_sprite.setPosition(this->window_->getSize().x/2 - this->message_screen_sprite.getGlobalBounds().width/2, 80.f);
//     this->messages_[1].setPosition(this->window_->getSize().x/2 - this->messages_[1].getGlobalBounds().width/2, 300.f);
//     this->messages_[0].setPosition(this->window_->getSize().x/2 - this->messages_[0].getGlobalBounds().width/2, 150.f);
// }

// void Game::shift_option_down() {
//     this->held_down = true;
//     this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//     this->home_option_selected++;
//     this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
// }

// void Game::shift_option_up() {
//     this->held_down = true;
//     this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//     this->home_option_selected--;
//     this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
// }

// void Game::select_using_mouse() {
//     sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(*this->window_).x, sf::Mouse::getPosition(*this->window_).y);
//     if(this->home_sprites[0].getGlobalBounds().contains(mouse_position)){
//         this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//         this->home_option_selected = 0;
//         this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
//         this->mouse_over = true;
//     }
//     else if(this->home_sprites[1].getGlobalBounds().contains(mouse_position)){
//         this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//         this->home_option_selected = 1;
//         this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
//         this->mouse_over = true;
//     }
//     else if(this->home_sprites[2].getGlobalBounds().contains(mouse_position)){
//         this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 250.f, 479.f, 200.f));
//         this->home_option_selected = 2;
//         this->home_sprites[this->home_option_selected].setTextureRect(sf::IntRect(0.f, 0.f, 479.f, 200.f));
//         this->mouse_over = true;
//     }
// }