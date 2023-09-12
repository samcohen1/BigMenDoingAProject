#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.h"
#include "Bullet.h"
#include "Splash_Screen.h"
#include "About_Screen.h"
#include "How_To_Play_Screen.h"
#include "Player.h"
#include "Professor.h"
#include "Professor_Assignment.h"
#include "Enemy.h"
#include "Throwable.h"


// //Tests for the bullet class
// TEST_CASE("Testing Bullet Constructor") {
//     sf::Texture texture;
//     sf::FloatRect world_bounds(0.f, 0.f, 800.f, 600.f);
//     Bullet bullet(100.f, 100.f, Direction::RIGHT, texture, world_bounds);

//     CHECK(bullet.get_location() == sf::Vector2f(100.f, 100.f));
// }

// TEST_CASE("Testing Bullet move method for one direction") {
//     sf::Texture texture;
//     sf::FloatRect world_bounds(0.f, 0.f, 800.f, 600.f);
//     Bullet bullet(100.f, 100.f, Direction::RIGHT, texture, world_bounds);

//     bullet.move(10.f);

//     CHECK(bullet.get_location().x == doctest::Approx(99.f));
//     CHECK(bullet.get_world_bounds().left == doctest::Approx(-1.f));
// }

// TEST_CASE("Testing Bullet move method for other direction") {
//     sf::Texture texture;
//     sf::FloatRect world_bounds(0.f, 0.f, 800.f, 600.f);
//     Bullet bullet(100.f, 100.f, Direction::LEFT, texture, world_bounds);

//     bullet.move(10.f);

//     CHECK(bullet.get_location().x == doctest::Approx(101.f));
//     CHECK(bullet.get_world_bounds().left == doctest::Approx(1.f));
// }

// TEST_CASE("Testing Bullet get_location method") {
//     sf::Texture texture;
//     sf::FloatRect world_bounds(0.f, 0.f, 800.f, 600.f);
//     Bullet bullet(100.f, 100.f, Direction::RIGHT, texture, world_bounds);

//     CHECK(bullet.get_location() == sf::Vector2f(100.f, 100.f));
// }

// TEST_CASE("Testing Bullet get_bounds method") {
//     sf::Texture texture;
//     sf::FloatRect world_bounds(0.f, 0.f, 800.f, 600.f);
//     Bullet bullet(100.f, 100.f, Direction::RIGHT, texture, world_bounds);

//     CHECK(bullet.get_bounds().left == doctest::Approx(100.f));
//     CHECK(bullet.get_bounds().top == doctest::Approx(100.f));
// }

// TEST_CASE("Testing Bullet get_world_bounds method") {
//     sf::Texture texture;
//     sf::FloatRect world_bounds(0.f, 0.f, 800.f, 600.f);
//     Bullet bullet(0.f, 0.f, Direction::RIGHT, texture, world_bounds);

//     CHECK(bullet.get_world_bounds() == world_bounds);
//     CHECK(bullet.get_world_bounds().left == 0.f);
// }


// //Tests for the professor class
// TEST_CASE("Testing Professor parameterized constructor") {
//     sf::Texture texture; 
//     Professor professor(texture);
//     CHECK(professor.get_num_professors() == 1); 
// }

// TEST_CASE("Testing Professor destructor") {
//     {
//         sf::Texture texture; 
//         Professor professor(texture);
//     } // professor goes out of scope and destructor is called here
//     CHECK(Professor::get_num_professors() == 0); 
// }

// TEST_CASE("Testing that init_professor method puts the professor in bounds") {
//     sf::Texture texture; 
//     Professor professor(texture);

// }