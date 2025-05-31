#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // Seed random number generator 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Create and run the game
    // The Game object's lifetime is managed here. When main ends, game_instance is destructed.
    // All unique_ptrs owned by game_instances will be cleaned up
    Game visitorCenterGame;
    visitorCenterGame.run();

    return 0;
}

