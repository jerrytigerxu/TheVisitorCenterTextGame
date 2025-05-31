#include "Game.h"
#include <iostream>
#include <algorithm>

// Constructor
Game::Game()
    : player(nullptr), // Player needs a starting room, will be set in setupGame
    guide("The Visitor Guide"),
    currentGameState(GameState::INTRO),
    gameOver(false) {
        setupGame();
}

// Initializes game objects
void Game::setupGame() {
    // TODO
}

void Game::setupRoomsAndExits() {
    // TODO
}

void Game::setupItems() {
    // TODO
}

void Game::setupInteractiveElements() {
    // TODO
}

void Game::setupGuide() {
    // Guide is already a member, initialized
    // Additional setup if Guide needs references to game systems or initial state based on game setup
    // For now, Guide::initializeDialogue() handles its internal setup
}

Room* Game::findRoomById(const std::string& roomId) {
    for (const auto& room_ptr : allRooms) {
        if (room_ptr->id == roomId) {
            return room_ptr.get();
        }
    }
    return nullptr;
}

void Game::displayIntro() {
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "           THE VISITOR CENTER (Beta Build A)             " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Your heart pounds with anxiety. Racing to your gravely ill mother, your chosen shortcut "
              << "has led to disaster. Your car sputters and dies near the Oakhaven Visitor Center – "
              << "an isolated, dilapidated structure exuding an unnerving stillness." << std::endl;
    std::cout << "Miles from anywhere, with a failing phone signal, the Center is your only hope." << std::endl;
    // Player's location is already set and looked at in setupGame.
    transitionToState(GameState::EXPLORING_CENTER_INITIAL);
}

void Game::transitionToState(GameState newState) {
    // TODO
}

void Game::displayEnding(GameState endingTyping) {
    switch (endingType) {
        case GameState::ENDING_NOT_WORTHY:
            std::cout << "\n--- ENDING 1: Not Worthy ---" << std::endl;
            std::cout << "The image of your mother, pale and still in a hospital bed, burned in your mind. Guilt was a hot stone in your gut, but the primal need to reach your family was a tidal wave." << std::endl;
            std::cout << "As you flee the Visitor Center, the Guide's voice, no longer weak, echoes from within:" << std::endl;
            std::cout << guide.name << ": \"Go then... Flee back to your... decaying world, and your precious attachments. Some souls... simply aren't worth preserving. You wouldn't have appreciated the artistry anyway.\"" << std::endl;
            std::cout << "You escape, the words a brand, but the drive to the hospital consuming all else." << std::endl;
            break;
        case GameState::ENDING_GOOD_ESCAPED:
            std::cout << "\n--- ENDING 2: Good Ending - Escaped ---" << std::endl;
            std::cout << "With a desperate surge of adrenaline, you use the " << (player.getItemFromInventory("surgical_item") ? player.getItemFromInventory("surgical_item")->name : "item") << "!" << std::endl;
            std::cout << "The Guide recoils, momentarily stunned, giving you the chance you need." << std::endl;
            std::cout << "You scramble past him and out of the horrific visitor center, not daring to look back." << std::endl;
            std::cout << "Forever scarred, you carry the weight of Oakhaven, but also a desperate hope as you race towards your mother." << std::endl;
            break;
        case GameState::ENDING_BAD_VICTIM:
            std::cout << "\n--- ENDING 3: Bad Ending - Victim ---" << std::endl;
            std::cout << "You try to react, but the Guide is too quick, his earlier frailty a complete deception." << std::endl;
            std::cout << "His smile is the last thing you see." << std::endl;
            std::cout << guide.name << ": \"Welcome,\" he whispers, \"to eternity. Such purity... preserved.\"" << std::endl;
            std::cout << "The Oakhaven Visitor Center had claimed another exhibit. Far away, a hospital vigil continued, unaware." << std::endl;
            break;
        default:
            std::cout << "Error: Unknown ending type." << std::endl;
            break;
    }
    gameOver = true;
    currentGameState = GameState::GAME_OVER;
}

// Main game loop
void Game::run() {
    displayIntro();
    std::string inputLine;

    while (!gameOver) {
        if (currentGameState == GameState::GAME_OVER) break;

        std::cout << "\n> ";
        if (!std::getline(std::cin, inputLine)) {
            if (std::cin.eof()) {
                std::cout << "EOF detected. Quitting." << std::endl;
                break;
            }
            std::cerr << "Input error. Quitting." << std::endl;
            break;
        }
        if (inputLine.empty()) continue;

        processInput(inputLine);
        updateGame();

    }

    if (currentGameState != GameState::GAME_OVER) {
        std::cout << "\n--- Game ended abruptly. ---" << std::endl;
    }
    std::cout << "\n--- Thank you for playing The Visitor Center! ---" << std::endl;
}

std::vector<std::string> Game::parseCommand(const std::string& rawInput) {
    // TODO
}

// Processes player input 
void Game::processInput(const std::string& rawInput) {
    // TODO
}

// Command handlers 

// Updates game state
void Game::updateGame() {
    // TODO
}