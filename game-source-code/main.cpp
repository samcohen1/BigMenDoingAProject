/** \file pong-main.cpp
    \brief Contains the main function

    NB:to document global objects (functions, typedefs, enum, macros, etc), you must document the file in which they are defined. That is,
    you must provide this file comment block.
 */


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>



// Define some global constants
const float pi = 3.14159f; /**< Pi */
const int gameWidth = 800; /**< The width of the game screen */
const int gameHeight = 600; /**< The height of the game screen */
const float ballRadius = 10.f; /**< The radius of the ball */
const sf::Vector2f paddleSize{25, 100}; /**< Represents the dimensions of the paddle's rectangle */

/** \fn int main()
 *  \brief This function contains the majority of the code for the game
 *
 *  \return The application exit code: 0 for successful completion; a negative number to indicate an error
 *
 */
int main()
{

    return EXIT_SUCCESS;
}
