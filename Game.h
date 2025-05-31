#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "Room.h"
#include "Player.h"
#include "Item.h"
#include "Guide.h"
#include "InteractiveElement.h"

// GameState enum to manage distinct game phases and narrative progression
enum class GameState {
    INTRO,                          // Initial game setup, player learns premise
    EXPLORING_CENTER_INITIAL,       // Player explores first rooms, first Guide encounter
    PLAYER_HAS_SOME_MEANS,          // Player has found at least one "means to leave" item
    PLAYER_HAS_ALL_MEANS,           // Player has found all "means to leave" items
    OPTIONAL_SURGICAL_ITEM_NOTICEABLE, // Surgical item can be found
    
    GUIDE_STAGES_ATTACK_PRELUDE,    // Guide initiates his plan (dialogue before "sounds")
    GUIDE_STAGES_ATTACK_SOUNDS,     // Horrific sounds heard
    GUIDE_FEIGNING_INJURY,          // Player finds Guide "injured"
    
    CHOICE_POINT_LEAVE_OR_HELP,     // Player must decide to leave or help Guide
    
    PLAYER_CHOOSES_LEAVE_ENDING1_PRE, // Player decided to leave (leads to Ending 1)
    
    PLAYER_CHOOSES_HELP_SEARCH_MEDKIT, // Player decided to help, goes for First Aid Kit
    PLAYER_FOUND_MEDKIT,            // Player has acquired the First Aid Kit
    PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL, // Player returns, Guide is fine, monologue starts
    FIGURES_REVEALED,               // Nature of "exhibits" is known
    
    FINAL_CONFRONTATION_IMMINENT,   // Guide is about to attack
    PLAYER_USES_SURGICAL_ITEM_ENDING2_PRE, // Player uses item (leads to Ending 2)
    PLAYER_FAILS_DEFENSE_ENDING3_PRE,    // Player fails to defend (leads to Ending 3)

    ENDING_NOT_WORTHY,              // Ending 1
    ENDING_GOOD_ESCAPED,            // Ending 2
    ENDING_BAD_VICTIM,              // Ending 3
    
    GAME_OVER                       // Terminal state
};

// Manages the overall game state, objects, and game loop
class Game {
public: 
    // The Game class owns all Room objects
    std::vector<std::unique_pr<Room>> allRooms;

    // The Game class owns the Player object 
    Player player; 

    // The Game class owns the Guide object
    Guide guide;

    GameState currentGameState;
    bool gameOver;

    // Constructor
    Game();

    // Main game loop
    void run();

private:
    // Initializes game objects
    void setupGame();
    void setupRoomsAndExits();
    void setupItems();
    void setupInteractiveElements();
    void setupGuide();

    // Processes player input
    void processInput(const std::string& rawInput);
    std::vector<std::string> parseCommand(const std::string& rawInput);

    // Updates game state based on input and current conditions
    void updateGame();
    void transitionToState(GameState newState);

    // Display functions
    void displayIntro();
    void displayEnding(GameState endingType);

    // Command handlers
    void handleGoCommand(const std::vector<std::string>& words);
    void handleLookCommand(const std::vector<std::string>& words);
    void handleExamineCommand(const std::vector<std::string>& words);
    void handleGetCommand(const std::vector<std::string>& words);
    void handleInventoryCommand(const std::vector<std::string>& words);
    void handleTalkCommand(const std::vector<std::string>& words);
    void handleHelpCommand(const std::vector<std::string>& words);
    void handleUseCommand(const std::vector<std::string>& words);
    void handleChooseCommand(const std::vector<std::string>& words);

    // Utility
    Room* findRoomById(const std::string& roomId);



};


#endif // GAME_H