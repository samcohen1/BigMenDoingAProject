/** \file main.cpp
    \brief Contains the main function
 */


///////////////////////////////////////
///// HEADERS
///////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Splash_Screen.h"


/** \fn int main()
 *  \brief This function instantiates a splash screen object and runs it.
 *
 *  \return The application exit code: 0 for successful completion; a negative number to indicate an error
 *
 */
int main() {
    auto splash_screen = Splash_Screen();
    splash_screen.run();

    // auto game = Game{};
    // game.run();

    return EXIT_SUCCESS;
}
