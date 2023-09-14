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
#include<cmath>


//////////////////////////////////
//Tests for the bullet class
//////////////////////////////////
TEST_CASE("Testing Bullet Constructor creates bullet in the correct location") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    Bullet bullet(100.f, 450.f, Direction::RIGHT, texture, player_bounds);

    CHECK(bullet.get_location().x == 100.f);
    CHECK(bullet.get_location().y == 450.f);
}

TEST_CASE("Testing Bullet moves to the left correctly without background movement") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    // RIGHT indicates right background movement and left player movement
    Bullet bullet(300.f, 300.f, Direction::RIGHT, texture, player_bounds);
    // 0.f is passed in to indicate no background movement for this test
    bullet.move(0.f);

    CHECK(bullet.get_location().x == 299.f);
}

TEST_CASE("Testing Bullet moves to the right correctly without background movement") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    // LEFT indicates left background movement and right player movement
    Bullet bullet(300.f, 300.f, Direction::LEFT, texture, player_bounds);
    // 0.f is passed in to indicate no background movement for this test
    bullet.move(0.f);

    CHECK(bullet.get_location().x == 301.f);
}

TEST_CASE("Testing Bullet moves to the left correctly with background movement") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    // RIGHT indicates right background movement and left player movement
    Bullet bullet(300.f, 300.f, Direction::RIGHT, texture, player_bounds);
    // 10.f is passed in for background movement for this test
    bullet.move(10.f);

    CHECK(bullet.get_location().x == 309.f);
}

TEST_CASE("Testing Bullet moves to the right correctly with background movement") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    // LEFT indicates left background movement and right player movement
    Bullet bullet(300.f, 300.f, Direction::LEFT, texture, player_bounds);
    // 10.f is passed in for background movement for this test
    bullet.move(10.f);

    CHECK(bullet.get_location().x == 311.f);
}

TEST_CASE("Testing Bullet does not move vertically without background movement") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    Bullet bullet(300.f, 300.f, Direction::RIGHT, texture, player_bounds);
    // 0.f is passed in for background movement for this test
    bullet.move(0.f);

    CHECK(bullet.get_location().y == 300.f);
}

TEST_CASE("Testing Bullet does not move vertically with background movement") {
    sf::Texture texture;
    sf::FloatRect player_bounds(50.f, 50.f, 10.f, 10.f);
    Bullet bullet(300.f, 300.f, Direction::LEFT, texture, player_bounds);
    // 10.f is passed in for background movement for this test
    bullet.move(10.f);

    CHECK(bullet.get_location().y == 300.f);
}


//////////////////////////////////
//Tests for the Professor_Assignment class
//////////////////////////////////
TEST_CASE("Testing Profeessor assignment constructor creates the assignment at the correct location") {
    sf::Texture texture;
    sf::Vector2f player_location(50.f, 50.f);
    sf::Vector2f professor_location(100.f, 100.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);

    CHECK(assignment.get_location().x == 100.f);
    CHECK(assignment.get_location().y == 100.f);
}

TEST_CASE("Testing Profeessor assignment constructor creates the assignment at the correct angle in first quadrant") {
    sf::Texture texture;
    sf::Vector2f player_location(100.f, 100.f);
    sf::Vector2f professor_location(50.f, 50.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);

    CHECK(assignment.get_angle() == doctest::Approx(M_PI/4));
}

TEST_CASE("Testing Profeessor assignment moves at the correct speed in quadrant one without background movement") {
    sf::Texture texture;
    // Positions corrected towards head of player -> 20 is added to both x and y components
    sf::Vector2f player_location(80.f, 30.f);
    sf::Vector2f professor_location(50.f, 100.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);
    // 0.f is passed in for the background movement in this case
    assignment.move(0.f);

    CHECK(assignment.get_location().x == doctest::Approx(50.f+0.1414));
    CHECK(assignment.get_location().y == doctest::Approx(100.f-0.1414));
}

TEST_CASE("Testing Profeessor assignment moves at the correct speed in quadrant two without background movement") {
    sf::Texture texture;
    // Positions corrected towards head of player -> 20 is added to both x and y components
    sf::Vector2f player_location(30.f, 30.f);
    sf::Vector2f professor_location(100.f, 100.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);
    // 0.f is passed in for the background movement in this case
    assignment.move(0.f);

    CHECK(assignment.get_location().x == doctest::Approx(100.f-0.1414));
    CHECK(assignment.get_location().y == doctest::Approx(100.f-0.1414));
}

TEST_CASE("Testing Profeessor assignment moves at the correct speed in quadrant three without background movement") {
    sf::Texture texture;
    // Positions corrected towards head of player -> 20 is added to both x and y components
    sf::Vector2f player_location(10.f, 80.f);
    sf::Vector2f professor_location(100.f, 50.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);
    // 0.f is passed in for the background movement in this case
    assignment.move(0.f);

    CHECK(assignment.get_location().x == doctest::Approx(100.f-0.1627));
    CHECK(assignment.get_location().y == doctest::Approx(50.f+0.1162));
}

TEST_CASE("Testing Profeessor assignment moves at the correct speed in quadrant four without background movement") {
    sf::Texture texture;
    // Positions corrected towards head of player -> 20 is added to both x and y components
    sf::Vector2f player_location(80.f, 80.f);
    sf::Vector2f professor_location(50.f, 50.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);
    // 0.f is passed in for the background movement in this case
    assignment.move(0.f);

    CHECK(assignment.get_location().x == doctest::Approx(50.f+0.1414));
    CHECK(assignment.get_location().y == doctest::Approx(50.f+0.1414));
}

TEST_CASE("Testing Profeessor assignment moves at the correct speed with background movement") {
    sf::Texture texture;
    // Positions corrected towards head of player -> 20 is added to both x and y components
    sf::Vector2f player_location(80.f, 30.f);
    sf::Vector2f professor_location(50.f, 100.f);
    sf::FloatRect professor_world_position(50.f, 50.f, 50.f, 50.f);
    Professor_Assignment assignment(texture, professor_location, player_location, professor_world_position);
    // 0.f is passed in for the background movement in this case
    assignment.move(10.f);

    CHECK(assignment.get_location().x == doctest::Approx(60.f+0.1414));
}



//////////////////////////////////
//Tests for the player class
//////////////////////////////////
TEST_CASE("Testing that the player is constructed with the correct location - the right side of the screen at the edge") {
    sf::Texture texture;
    Player player(texture);
    //Player's default position is at the right edge and at a height of 300 from the top
    CHECK(player.get_position().x_right == doctest::Approx(player.get_x_default_right()));
    CHECK(player.get_position().y == 300.f);

}

TEST_CASE("Testing player's movement upwards, when key is pressed for one frame and player is not at its maximum position") {
     sf::Texture texture;
    Player player(texture);
    //The boolean value true is to show that the key is pressed and that he is not just slowing down when he has been moving but the key is no longer pressed
    player.move_player_vertical(Direction::UP, true);
    //When he moves up, the y-component decreases
    //Default speed when pressed is 0.2 per frame
    CHECK(player.get_position().y == 299.8f);
}

TEST_CASE("Testing player's movement upwards, when key is pressed for three frames and player is not at its maximum position") {
    sf::Texture texture;
    Player player(texture);
    for(auto i = 0; i<3; i++){
        player.move_player_vertical(Direction::UP, true);
    }

    CHECK(player.get_position().y == doctest::Approx(299.4));
}

TEST_CASE("Testing player's movement upwards, when key is not pressed and player is not at its maximum position") {
     sf::Texture texture;
    Player player(texture);
    //The boolean value false is to show that the key is not pressed but the player is moving
    player.move_player_vertical(Direction::UP, false);
    //When he moves up, the y-component decreases
    //Default speed when pressed is 0.2 per frame and acceleration vertically is 0.0004
    CHECK(player.get_position().y == 299.8004f);
}

TEST_CASE("Testing player's movement upwards, when key is not pressed for thre frames and player is not at its maximum position") {
     sf::Texture texture;
    Player player(texture);
    
    for(auto i = 0; i<3; i++){
        player.move_player_vertical(Direction::UP, false);
    }
    CHECK(player.get_position().y == doctest::Approx(299.4024));
    
}


TEST_CASE("Testing player's movement downwards, when key is pressed for one frame and player is not at its maximum position") {
     sf::Texture texture;
    Player player(texture);
    //The boolean value true is to show that the key is pressed and that he is not just slowing down when he has been moving but the key is no longer pressed
    player.move_player_vertical(Direction::DOWN, true);
    //When he moves down, the y-component increases
    //Default speed when pressed is 0.2 per frame
    CHECK(player.get_position().y == 300.2f);
}

TEST_CASE("Testing player's movement upwards, when key is pressed for three frames and player is not at its maximum position") {
    sf::Texture texture;
    Player player(texture);
    for(auto i = 0; i<3; i++){
        player.move_player_vertical(Direction::DOWN, true);
    }

    CHECK(player.get_position().y == doctest::Approx(300.6));
}

TEST_CASE("Testing player's movement upwards, when key is not pressed and player is not at its maximum position") {
     sf::Texture texture;
    Player player(texture);
    //The boolean value false is to show that the key is not pressed but the player is moving
    player.move_player_vertical(Direction::DOWN, false);
    //When he moves up, the y-component decreases
    //Default speed when pressed is 0.2 per frame and acceleration vertically is 0.0004
    CHECK(player.get_position().y == 300.1996f);
}

TEST_CASE("Testing player's movement upwards, when key is not pressed for thre frames and player is not at its maximum position") {
     sf::Texture texture;
    Player player(texture);
    
    for(auto i = 0; i<3; i++){
        player.move_player_vertical(Direction::DOWN, false);
    }
    CHECK(player.get_position().y == doctest::Approx(300.5976));
    
}


TEST_CASE("Testing that a player will reach a maximum height and cannot go past that height") {
    sf::Texture texture;
    Player player(texture);

    for(auto i = 0; i<100000; i++){
         player.move_player_vertical(Direction::UP, true);
    }
    CHECK(player.get_position().y == doctest::Approx(120));
}

TEST_CASE("Testing that a player will reach a minimum height and cannot go below that height") {
    sf::Texture texture;
    Player player(texture);

    for(auto i = 0; i<100000; i++){
         player.move_player_vertical(Direction::DOWN, true);
    }
    CHECK(player.get_position().y == doctest::Approx(800-42.97f));
}

TEST_CASE("Testing that if a key is not pressed but the player is moving upwards it will come to a stop after some deceleration") {
    sf::Texture texture;
    Player player(texture);

    for(auto i = 0; i<100000; i++){
         player.move_player_vertical(Direction::UP, false);
    }
    CHECK(player.get_position().y > 120);
    CHECK(player.get_position().y < 300);
}

TEST_CASE("Testing that if a key is not pressed but the player is moving downwards it will come to a stop after some deceleration") {
    sf::Texture texture;
    Player player(texture);

    for(auto i = 0; i<100000; i++){
         player.move_player_vertical(Direction::DOWN, false);
    }
    CHECK(player.get_position().y > 300);
    CHECK(player.get_position().y < 700);
}

TEST_CASE("Testing that a player will move to the left side of the screen when the left command is given") {
    sf::Texture texture;
    Player player(texture);
    player.move_player_horizontal(Direction::LEFT);
    CHECK(player.get_position().x_right == doctest::Approx(player.get_x_default_right()-0.1508));
}

TEST_CASE("Testing that a player will move to the left side of the screen when the left arrow is held") {
    sf::Texture texture;
    Player player(texture);
    for(auto i = 0; i<3; i++){
        player.move_player_horizontal(Direction::LEFT);
    }
    CHECK(player.get_position().x_right == doctest::Approx(player.get_x_default_right()-0.4548));
}

TEST_CASE("Testing that a player will move to the right side of the screen when the right command is given") {
    sf::Texture texture;
    Player player(texture);
    player.move_player_horizontal(Direction::RIGHT);
    CHECK(player.get_position().x_right == doctest::Approx(player.get_x_default_right()+0.1508));
}

TEST_CASE("Testing that a player will move to the right side of the screen when the right button is held") {
    sf::Texture texture;
    Player player(texture);
    for(auto i = 0; i<3; i++){
        player.move_player_horizontal(Direction::RIGHT);
    }
    CHECK(player.get_position().x_right == doctest::Approx(player.get_x_default_right()+0.4548));
}

TEST_CASE("Testing that a player will be magnetised to the left side of the screen if the last move was to the left and the player is in the middle of the screen") {
    sf::Texture texture;
    Player player(texture);
    for(auto i = 0; i<1000; i++){
        player.move_player_horizontal(Direction::LEFT);
    }
    CHECK(player.get_position().x_left > player.get_x_default_left());
    for(auto i = 0; i<5000; i++){
        player.magnetise_player();
    }
    CHECK(player.get_position().x_left <= player.get_x_default_left());
}

TEST_CASE("Testing that a player will be magnetised to the right side of the screen if the last move was right and the player is in the middle of the screen") {
    sf::Texture texture;
    Player player(texture);
    for(auto i = 0; i<1000; i++){
        player.move_player_horizontal(Direction::LEFT);
    }
    player.move_player_horizontal(Direction::RIGHT);
    CHECK(player.get_position().x_right < player.get_x_default_right());
    for(auto i = 0; i<5000; i++){
        player.magnetise_player();
    }
    CHECK(player.get_position().x_right >= player.get_x_default_right());
}

TEST_CASE("Testing that shooting a bullet will increase the size of the player's bullets vector at the start") {
    //The cool down starts at 400 and the player is able to shoot and the cooldown resets
    sf::Texture texture;
    Player player(texture);
    CHECK(player.get_bullets().size() == 0);
    player.shoot_bullet(texture);
    CHECK(player.get_bullets().size() == 1);
}

TEST_CASE("Testing that shooting a bullet will not increase the size of the player's bullets vector if the cooldown has not yet been reached") {
    //The cool down starts at 400 and the player is able to shoot and the cooldown resets
    sf::Texture texture;
    Player player(texture);
    player.shoot_bullet(texture);
    CHECK(player.get_bullets().size() == 1);
    player.shoot_bullet(texture);
    CHECK(player.get_bullets().size() == 1);

}

TEST_CASE("Testing that shooting a bullet will increase the size of the player's bullets vector if the cooldown value has been reached") {
    //The cool down starts at 400 and the player is able to shoot and the cooldown resets
    sf::Texture texture;
    Player player(texture);
    player.shoot_bullet(texture);
    for(auto i = 0; i<400; i++){
        player.increment_cool_down();
    }
    player.shoot_bullet(texture);
    CHECK(player.get_bullets().size() == 2);

}

TEST_CASE("Testing that erasing a bullet will decrease the size of the player's bullets vector") {
    sf::Texture texture;
    Player player(texture);
    player.shoot_bullet(texture);
    player.erase_bullet(player.get_bullets()[0]);
    CHECK(player.get_bullets().size() == 0);

}

//////////////////////////////////
//Tests for the player class
//////////////////////////////////
TEST_CASE("check that the constructor creates a professor within vertical bounds") {
    sf::Texture texture;
    auto professor = Professor(texture);

    CHECK(professor.get_location().y >= 120);
    CHECK(professor.get_location().y <= 760);
}

TEST_CASE("check that the constructor creates a professor within horizontal bounds") {
    sf::Texture texture;
    auto professor = Professor(texture);

    CHECK(professor.get_location().x >= -2777);
    CHECK(professor.get_location().x <= 5650);
}

TEST_CASE("check that the static count of professors increments upon contruction") {
    // check for no professors before construction
    CHECK(Professor::get_num_professors() == 0);
    sf::Texture texture;
    auto professor = Professor(texture);

    CHECK(Professor::get_num_professors() == 1);
}

TEST_CASE("check that the static count of professors decrements upon deletion") {
    {
        sf::Texture texture;
        auto professor = Professor(texture);
        CHECK(Professor::get_num_professors() == 1);
    }
    // once out of scope check for decrement
    CHECK(Professor::get_num_professors() == 0);
}

TEST_CASE("check that the professor moves horizonatally without background movement") {
    sf::Texture texture;
    auto professor = Professor(texture);
    auto player_position = sf::Vector2f(10.f, 10.f);
    // two zero floats inputed for no background movement
    auto x_initial = professor.get_location().x;
    professor.move(0.f, 0.f, player_position);

    // check for either bi-directional movement (direction randomly initialised)
    bool correct_movement = professor.get_location().x == doctest::Approx(x_initial + 0.03) || professor.get_location().x == doctest::Approx(x_initial - 0.03);
    CHECK(correct_movement);
}

TEST_CASE("check that the professor moves horizonatally with background movement") {
    sf::Texture texture;
    auto professor = Professor(texture);
    auto player_position = sf::Vector2f(10.f, 10.f);
    // background values set as 10 for this test
    auto x_initial = professor.get_location().x;
    professor.move(10.f, 10.f, player_position);

    // check for either bi-directional movement (direction randomly initialised)
    bool correct_movement = professor.get_location().x == doctest::Approx(x_initial + 10.03) || professor.get_location().x == doctest::Approx(x_initial +9.97);
    CHECK(correct_movement);
}

TEST_CASE("check that the professor moves within vertical bounds over time") {
    sf::Texture texture;
    auto professor = Professor(texture);
    auto player_position = sf::Vector2f(10.f, 10.f);
    bool within_bounds = true;
    for (int i = 0; i < 1000; i++) {
        professor.move(0.f, 0.f, player_position);
        if (!(professor.get_location().y >= 100 && professor.get_location().y <= 800)) within_bounds = false;
    }

    CHECK(within_bounds);
}

TEST_CASE("check that the professor moves within horizontal bounds over time") {
    sf::Texture texture;
    auto professor = Professor(texture);
    auto player_position = sf::Vector2f(10.f, 10.f);
    bool within_bounds = true;
    for (int i = 0; i < 100000; i++) {
        professor.move(0.f, 0.f, player_position);
        if (!(professor.get_location().x >= -2800 && professor.get_location().x <= 5700)) within_bounds = false;
    }

    CHECK(within_bounds);
}

TEST_CASE("check that the professor is dying after first getting destroyed destroyed") {
    sf::Texture texture;
    auto professor = Professor(texture);
    // check not dying before shot
    CHECK_FALSE(professor.is_dying());
    professor.destroy();
    CHECK(professor.is_dying());
}

TEST_CASE("check that the professor is dead after a certain amount of time dying") {
    sf::Texture texture;
    auto professor = Professor(texture);
    // check not dying before shot
    professor.destroy();
    CHECK_FALSE(professor.get_is_dead());
    for (int i = 0; i < 500; i++) {
        professor.destroy();
    }
    CHECK(professor.get_is_dead());
}

//////////////////////////////////
//Tests for the game class
//////////////////////////////////

TEST_CASE("Testing that the professor vector is incremented when the professor cool down has been reached") {

    std::shared_ptr<sf::RenderWindow> window;
    auto game = Game(window, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f);

    //The cool down is 100, therefore it needs to run over 100 times
    for(auto i = 0; i<105; i++){
        game.teleport_enemies(3);
    }
    CHECK(game.get_enemies().size() == 1);
}


TEST_CASE("Testing that the professor vector is incremented again when cool down is reached, if the maximum number of professors has not been reached") {
    std::shared_ptr<sf::RenderWindow> window;
    auto game = Game(window, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f);

    //3 is the maximum number of professors
    for(auto i = 0; i<220; i++){
        game.teleport_enemies(3);
    }
    CHECK(game.get_enemies().size()==2);
}


TEST_CASE("Testing that the professor vector is not incremented when the maximum number of professors have been reached") {

    std::shared_ptr<sf::RenderWindow> window;
    auto game = Game(window, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f);

    //1 is the maximum number of professors
    for(auto i = 0; i<220; i++){
        game.teleport_enemies(1);
    }
    CHECK(game.get_enemies().size()==1);
}

TEST_CASE("Testing the erase enemy function - that it removes an enemy after being shot") {

    std::shared_ptr<sf::RenderWindow> window;
    auto game = Game(window, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f);

    //1 is the maximum number of professors
    for(auto i = 0; i<220; i++){
        game.teleport_enemies(3);
    }

    game.erase_enemy(0);
    CHECK(game.get_enemies().size()==1);
}