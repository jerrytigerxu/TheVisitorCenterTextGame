#include <iostream>
#include <string>
#include <vector>
#include <map>

/**
 * \@class Room
 * \@brief Represents a location in the game
 * 
 * For the Alpha phase, a room only needs a name
 * Connections will be handled by string keys for simplicity in Alpha-A
 * Placeholder for description for now
 */
class Room {
    public:
        std::string name;
        std::string description; // Placeholder for now
        // For now, Room will just have a name and a basic description

        // Constructor
        Room(std::string n = "Unnamed Room", std::string d = "A non-descript location.") 
            : name(std::move(n)), description(std::move(d)) {}
        
        // Display room information
        void look() const {
            std::cout << "\n--- " << name << " ---" << std::endl;
            std::cout << description << std::endl;
        }
};

/**
 * \@class Player
 * \@brief Represents the player
 * 
 * For the Alpha phase, the player only needs to know their current location
 * represented by an index into the game's list of rooms
 */
class Player {
    public:
        int currentLocationIndex;

        // Constructor
        Player(int startLocationIndex = 0) : currentLocationIndex(startLocationIndex) {}

        // Method to change player's location
        void moveTo(int newLocationIndex) {
            currentLocationIndex = newLocationIndex;
        }
};

/**
 * \@class Game
 * \@brief Manages the overall game state
 * 
 * For the Alpha phase, this class sets up the necessary rooms and the player. 
 * It will have a very basic structure for the game loop and story progression
 */
class Game {
    public:
        std::vector<Room> rooms;
        Player player;
        bool gameOver;

        // Constructor
        Game() : player(0), gameOver(false) {
            setupRooms();
        }

        /**
         * \@brief Creates the basic rooms needed for the Alpha skeleton
         * 
         * Placeholder rooms: {0, Car Breakdown}, {1, Visitor Center Entrance}, {2, Main Hall},
         * {3, Storage}, {4, Office}, {5, Crisis Room}, {6, Reveal Spot}
         */
        void setupRooms() {
            rooms.emplace_back("Car Breakdown", "[Placeholder] Your car has sputtered to a halt. The imposing Oakhaven Visitor Center is your only option.");
            rooms.emplace_back("Visitor Center Entrance", "[Placeholder] You stand at the threshold of the visitor center. It feels cold.");
            rooms.emplace_back("Main Hall", "[Placeholder] A large, dusty main hall. An old 'Guide' is here.");
            rooms.emplace_back("Storage Room", "[Placeholder] A cluttered storage area. You might find car parts here.");
            rooms.emplace_back("Office", "[Placeholder] An old office. Perhaps a First Aid Kit could be here?");
            rooms.emplace_back("Crisis Room", "[Placeholder] A specific location where the 'staged attack' will occur.");
            rooms.emplace_back("Reveal Spot", "[Placeholder] The place where the Guide reveals his true nature.");
        }

        /**
         * \@brief Displays the intro text for the game (Stage 1)
         */
        void displayIntro() {
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << "           THE VISITOR CENTER (Alpha Build)             " << std::endl;
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << "[Placeholder Intro Text - Game Stage 1]" << std::endl;
            std::cout << "You were racing to your gravely ill parent, took a shortcut, and now your car is dead." << std::endl;
            std::cout << "The Oakhaven Visitor Center looms before you. It's your only hope." << std::endl;
            rooms[player.currentLocationIndex].look();
        }

        /**
         * \@brief Starts and runs the game. For Alpha-A, this is just a setup check. 
         * For Alpha-B/C, this will contain the main game loop.
         */
        void run() {
            std::cout << "--- Game Initializing (Alpha-A Check) ---" << std::endl;
            if (!rooms.empty()) {
                std::cout << "Player starting location: " << rooms[player.currentLocationIndex].name << std::endl;
                rooms[player.currentLocationIndex].look();
            } else {
                std::cout << "Error: No rooms defined!" << std::endl;
                return;
            }
            std::cout << "Total rooms defined: " << rooms.size() << std::endl;
            std::cout << "Alpha-A: Minimal classes (Room, Player, Game) are defined." << std::endl;
            std::cout << "Makefile and main.cpp are ready." << std::endl;
            std::cout << "Next step will be implementing the barebones story path (Alpha-B)." << std::endl;
            std::cout << "-----------------------------------------------------------------" << std::endl;
        
            // The actual game loop will be fleshed out in Alpha-B and Alpha-C
            // displayIntro(); // This will be part of the actual game loop later.
        }

};

 int main() {
    Game visitorCenterGame;
    visitorCenterGame.run();
    return 0;
 }