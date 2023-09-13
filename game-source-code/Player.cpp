/** \file Player.cpp
    \brief Contains the implementation of the player
 */

///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "Bullet.h"

/** \class Player
 *  \brief Represents the player character in the game.
 */
Player::Player(sf::Texture& texture) {
    this->init_player(texture);
}

/** \fn void Player::init_player(sf::Texture& texture)
 *  \brief Initialize the player character.
 *  \param texture The texture for the player's sprite.
 *  This function initializes the player character with the given texture and sets its initial properties.
 */
void Player::init_player(sf::Texture& texture) {
    this->player_sprite_.setTexture(texture);
    this->player_sprite_.setTextureRect(sf::IntRect(1054.f, 0.f, 985.f, 414.f));
    this->player_sprite_.setScale(this->scale_player_, this->scale_player_);
    this->player_sprite_.setPosition(this->x_default_right_ - this->player_sprite_.getGlobalBounds().width, this->default_y_);
    this->prev_direction_ = Direction::LEFT;
    this->prev_vertical_direction_ = Direction::LEFT;
    this->flip_player();
}

/** \fn void Player::flip_player()
 *  \brief Flip the player character to change direction.
 *  This function flips the player's sprite horizontally to change its facing direction.
 */
void Player::flip_player() {
    this->player_speed_ = this->flip_base_speed_;
    float prev_direction_flag = static_cast<float>(this->prev_direction_);
    this->player_sprite_.setScale(-1.f * this->scale_player_ * prev_direction_flag, this->scale_player_);
    this->player_sprite_.setOrigin(((prev_direction_flag + 1) / 2.f) * this->player_sprite_.getGlobalBounds().width / this->scale_player_, 0.f);
    this->prev_direction_ = static_cast<Direction>(-1 * prev_direction_flag);
}

/** \fn bool Player::direction_changed(Direction direction)
 *  \brief Check if the direction of the player character has changed.
 *  \param direction The new direction to check against.
 *  \return True if the direction has changed; false otherwise.
 *  This function checks if the direction of the player character has changed compared to the provided direction.
 */
bool Player::direction_changed(Direction direction) { return this->prev_direction_ != direction; }

/** \fn void Player::move_player_vertical(Direction direction, bool pressed)
 *  \brief Move the player character vertically.
 *  \param direction The direction of movement (UP or DOWN).
 *  \param pressed True if the movement key is pressed; false otherwise.
 *  This function updates the vertical position of the player character based on its direction and key press status.
 */
void Player::move_player_vertical(Direction direction, bool pressed) {
    float bottom = this->player_sprite_.getGlobalBounds().top + this->player_sprite_.getGlobalBounds().height + 10.f;
    if (pressed)
        this->vertical_speed_ = this->vertical_base_speed;
    else if (this->vertical_speed_ > 0)
        this->vertical_speed_ -= this->vertical_acceleration_;

    switch (direction) {
        case Direction::UP:
            if (this->player_sprite_.getGlobalBounds().top > this->y_min_pos_) {
                this->player_sprite_.move(0.f, -this->vertical_speed_);
            }
            this->prev_vertical_direction_ = Direction::UP;
            break;
        case Direction::DOWN:
            if (bottom < this->y_max_pos_) {
                this->player_sprite_.move(0.f, this->vertical_speed_);
            }
            this->prev_vertical_direction_ = Direction::DOWN;
            break;
        default:
            return;
    }
}

/** \fn void Player::move_player_horizontal(Direction direction)
 *  \brief Move the player character horizontally.
 *  \param direction The direction of movement (LEFT or RIGHT).
 *  This function updates the horizontal position of the player character using its direction.
 */
void Player::move_player_horizontal(Direction direction) {
    this->player_speed_ += this->acceleration_;
    this->player_sprite_.move(-static_cast<float>(direction) * this->player_speed_, 0.f);
    if (this->direction_changed(direction))
        this->flip_player();
    this->prev_direction_ = direction;
}



/** \fn void Player::correct_edge_positions()
 *  \brief Correct the player character's position at the screen edge.
 *  This function corrects the position of the player character to the edge of the screen.
 */
void Player::correct_edge_positions() {
    if (this->player_sprite_.getGlobalBounds().left < 700.f) {
        this->player_sprite_.setPosition(this->x_default_left_ - this->player_sprite_.getGlobalBounds().width, this->player_sprite_.getGlobalBounds().top);
        return;
    }
    this->player_sprite_.setPosition(this->x_default_right_, this->player_sprite_.getGlobalBounds().top);
}

/** \fn void Player::magnetise_player()
 *  \brief Magnetize the player character towards the edge.
 *  This function magnetizes the player character towards the edge when it's in the middle.
 */
void Player::magnetise_player() {
    float acceleration_constant = 0.1f;

    if ((this->player_sprite_.getGlobalBounds().left <= this->x_default_left_ && this->prev_direction_ == Direction::RIGHT)) {
        float delta_x = -((this->player_sprite_.getGlobalBounds().left + this->player_sprite_.getGlobalBounds().width) - this->x_default_right_);
        float acceleration = acceleration_constant / delta_x;
        this->player_speed_ += acceleration;
        this->player_sprite_.move(-static_cast<float>(this->prev_direction_) * this->player_speed_, 0.f);
        return;
    } else if (this->player_sprite_.getGlobalBounds().left + this->player_sprite_.getGlobalBounds().width >= this->x_default_right_ && this->prev_direction_ == Direction::LEFT) {
        float delta_x = this->player_sprite_.getGlobalBounds().left - this->x_default_left_;
        float acceleration = acceleration_constant / delta_x;
        this->player_speed_ += acceleration;
        this->player_sprite_.move(-static_cast<float>(this->prev_direction_) * this->player_speed_, 0.f);
        return;
    }

    if (this->player_sprite_.getGlobalBounds().left <= this->x_default_left_ || this->player_sprite_.getGlobalBounds().left + this->player_sprite_.getGlobalBounds().width >= this->x_default_right_) {
        return;
    }

    float delta_x = this->prev_direction_ == Direction::LEFT ? (this->player_sprite_.getGlobalBounds().left - this->x_default_left_) : -((this->player_sprite_.getGlobalBounds().left + this->player_sprite_.getGlobalBounds().width) - this->x_default_right_);
    float acceleration = acceleration_constant / delta_x;
    this->player_speed_ += acceleration;
    this->player_sprite_.move(-static_cast<float>(this->prev_direction_) * this->player_speed_, 0.f);
}

/** \fn void Player::render_bullets(sf::RenderTarget &target, float background_movement)
 *  \brief Render the player's bullets on the screen.
 *  \param target The render target to draw on.
 *  \param background_movement The movement of the game background.
 *  This function renders the player's bullets on the screen.
 */
void Player::render_bullets(sf::RenderTarget &target, float background_movement) {
    for (auto i = 0; i < this->bullets_.size(); i++) {
        if (this->bullets_[i]->get_location().x > 0.f && this->bullets_[i]->get_location().x < 1400.f) {
            this->bullets_[i]->move(background_movement);
            this->bullets_[i]->draw(target);
        } else
            erase_bullet(i);
    }
}

/** \fn void Player::render(sf::RenderTarget &target, float background_movement)
 *  \brief Render the player character and bullets on the screen.
 *  \param target The render target to draw on.
 *  \param background_movement The movement of the game background.
 *  This function renders the player character and bullets on the screen.
 */
void Player::render(sf::RenderTarget &target, float background_movement) {
    target.draw(player_sprite_);
    this->render_bullets(target, background_movement);
}

/** \fn float Player::get_x_default_right() const
 *  \brief Get the default right position of the player character.
 *  \return The default right position.
 *  This function returns the default right position of the player character.
 */
float Player::get_x_default_right() const { return this->x_default_right_; }

/** \fn float Player::get_x_default_left() const
 *  \brief Get the default left position of the player character.
 *  \return The default left position.
 *  This function returns the default left position of the player character.
 */
float Player::get_x_default_left() const { return this->x_default_left_; }

/** \fn float Player::get_player_speed() const
 *  \brief Get the speed of the player character.
 *  \return The speed of the player character.
 *  This function returns the speed of the player character.
 */
float Player::get_player_speed() const { return this->player_speed_ * static_cast<float>(this->prev_direction_); }

/** \fn Position Player::get_position()
 *  \brief Get the current position of the player character.
 *  \return The current position.
 *  This function returns the current position of the player character.
 */
Position Player::get_position() {
    auto global_position = Position();
    global_position.x_left = this->player_sprite_.getGlobalBounds().left;
    global_position.x_right = this->player_sprite_.getGlobalBounds().left + this->player_sprite_.getGlobalBounds().width;
    global_position.y = this->player_sprite_.getGlobalBounds().top;
    return global_position;
}

/** \fn std::vector<std::shared_ptr<Bullet>> Player::get_bullets() const
 *  \brief Get the player's bullets.
 *  \return A vector of shared pointers to the player's bullets.
 *  This function returns a vector of shared pointers to the player's bullets.
 */
std::vector<std::shared_ptr<Bullet>> Player::get_bullets() const { return this->bullets_; }

/** \fn void Player::shoot_bullet(sf::Texture& texture)
 *  \brief Shoot a bullet from the player character.
 *  \param texture The texture for the bullet.
 *  This function allows the player character to shoot a bullet.
 */
void Player::shoot_bullet(sf::Texture& texture) {
    if (this->current_cool_down < this->max_cool_down)
        return;
    else
        this->current_cool_down = 0;

    this->player_sprite_.setTextureRect(sf::IntRect(0.f, 0.f, 985.f, 414.f));
    auto x_position = 0.f;
    auto y_position = this->player_sprite_.getGlobalBounds().top + this->player_sprite_.getGlobalBounds().height - 11.f;

    if (this->prev_direction_ == Direction::LEFT) {
        x_position = this->player_sprite_.getGlobalBounds().left + 1 * this->player_sprite_.getGlobalBounds().width;
    } else
        x_position = this->player_sprite_.getGlobalBounds().left;

    this->bullets_.push_back(std::make_shared<Bullet>(x_position, y_position, this->prev_direction_, texture, this->world_bounds_));
}

/** \fn void Player::increment_cool_down()
 *  \brief Increment the shooting cooldown for the player character.
 *  This function increments the shooting cooldown timer for the player character.
 */
void Player::increment_cool_down() {
    if (this->current_cool_down > 100)
        this->player_sprite_.setTextureRect(sf::IntRect(1054.f, 0.f, 985.f, 414.f));
    this->current_cool_down++;
}

/** \fn void Player::erase_bullet(int position)
 *  \brief Erase a bullet from the player's bullets.
 *  \param position The position of the bullet to erase.
 *  This function erases a bullet from the player's bullets vector.
 */
void Player::erase_bullet(int position) {
    this->bullets_.erase(this->bullets_.begin() + position);
}

/** \fn void Player::erase_bullet(int position)
 *  \brief Erase a bullet from the player's bullets.
 *  \param bullet a pinter to the bullet to erase.
 *  This function erases a bullet from the player's bullets vector.
 */
void Player::erase_bullet(std::shared_ptr<Bullet> bullet) {
    auto it = std::find(this->bullets_.begin(), this->bullets_.end(), bullet);
    if(it != this->bullets_.end()) {
        this->bullets_.erase(it);
    } else std::cout << "bullet non existant cannot erase\n";
}

/** \fn Direction Player::get_prev_vertical_direction()
 *  \brief Get the previous vertical direction of the player character.
 *  \return The previous vertical direction (UP or DOWN).
 *  This function returns the previous vertical direction of the player character.
 */
Direction Player::get_prev_vertical_direction() { return this->prev_vertical_direction_; }

/** \fn void Player::communicate_position(float background_location)
 *  \brief Communicate the player character's position.
 *  \param background_location The location of the game background.
 *  This function communicates the position of the player character to the game, taking into account the background location.
 */
void Player::communicate_position(float background_location) {
    auto new_world_bounds = sf::FloatRect(this->player_sprite_.getGlobalBounds().left - background_location, this->player_sprite_.getGlobalBounds().top, this->player_sprite_.getGlobalBounds().width, this->player_sprite_.getGlobalBounds().height);
    this->world_bounds_ = new_world_bounds;
}

/** \fn sf::FloatRect Player::get_world_bounds()
 *  \brief Get the world bounds of the player character.
 *  \return The world bounds as an SFML FloatRect.
 *  This function returns the world bounds of the player character.
 */
sf::FloatRect Player::get_world_bounds() {
    return this->world_bounds_;
}
