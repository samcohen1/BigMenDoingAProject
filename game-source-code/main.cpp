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

#include "Game.h"


/** \fn int main()
 *  \brief This function contains the majority of the code for the game
 *
 *  \return The application exit code: 0 for successful completion; a negative number to indicate an error
 *
 */
int main() {
    auto game = Game{};
    game.run();

    return EXIT_SUCCESS;
}
