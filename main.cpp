#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

// Forward declaration
class Game; 

/**
 * \@class Room
 * \@brief Represents a location in the game
 * 
 */
class Room {
    public:
        std::string name;
        std::string description; // Placeholder for now
        std::string simplified_name;

        // Constructor
        Room(std::string n, std::string d, std::string sn) 
            : name(std::move(n)), description(std::move(d)), simplified_name(std::move(sn)) {}
        
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
        bool hasMeansToLeave;
        bool hasSurgicalItem;
        bool hasFirstAidKit;

        // Constructor
        Player(int startLocationIndex = 0) 
            : currentLocationIndex(startLocationIndex),
            hasMeansToLeave(false),
            hasSurgicalItem(false),
            hasFirstAidKit(false) {}

        // Method to change player's location
        void moveTo(int newLocationIndex) {
            currentLocationIndex = newLocationIndex;
        }
};

// Enum to manage simple game story progression for Alpha
enum class StoryBeat {
    INTRO,
    EXPLORING_CENTER,
    FOUND_ALL_MEANS_TO_LEAVE,
    GUIDE_STAGES_ATTACK,
    CHOICE_LEAVE_OR_HELP,
    PLAYER_SEARCHES_MEDKIT,
    PLAYER_FINDS_GUIDE_UNHARMED_REVEAL,
    FINAL_CONFRONTATION_LUNGE,
    ENDING_NOT_WORTHY,
    ENDING_GOOD_ESCAPED,
    ENDING_BAD_VICTIM,
    GAME_OVER
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
        StoryBeat currentStoryBeat;

        // Constructor
        Game() : player(0), gameOver(false), currentStoryBeat(StoryBeat::INTRO) {
            setupRooms();
        }

        /**
         * \@brief Creates the basic rooms needed for the Alpha skeleton
         * 
         * Placeholder rooms: {0, Car Breakdown}, {1, Visitor Center Entrance}, {2, Main Hall},
         * {3, Storage}, {4, Office}, {5, Crisis Room}, {6, Reveal Spot}
         */
        void setupRooms() {
            // Key locations for Alpha-B
            rooms.emplace_back("Car Breakdown", "[Placeholder] Your car has sputtered to a halt. The imposing Oakhaven Visitor Center is your only option.", "car");
            rooms.emplace_back("Visitor Center Entrance", "[Placeholder] You stand at the threshold of the visitor center. It feels cold.", "entrance");
            rooms.emplace_back("Main Hall", "[Placeholder] A large, dusty main hall. An old 'Guide' is here. He eyes you curiously.", "hall");
            rooms.emplace_back("Storage Room", "[Placeholder] A cluttered storage area. You might find car parts here.", "storage");
            rooms.emplace_back("Office", "[Placeholder] An old office. Perhaps a First Aid Kit could be here if needed?", "office");
            rooms.emplace_back("Crisis Room", "[Placeholder] A specific location where the 'staged attack' will occur. The Guide led you here.", "crisis"); // Player might be led here
            rooms.emplace_back("Reveal Spot", "[Placeholder] The place where the Guide reveals his true nature. You returned here after searching for the medkit.", "reveal"); // Often same as Crisis Room or Main Hall
    }

        /**
         * \@brief Displays the intro text for the game (Stage 1)
         */
        void displayIntro() {
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << "           THE VISITOR CENTER (Alpha Build - B)             " << std::endl;
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << "[Placeholder Intro Text - Game Stage 1]" << std::endl;
            std::cout << "You were racing to your gravely ill parent, took a shortcut, and now your car is dead." << std::endl;
            std::cout << "The Oakhaven Visitor Center looms before you. It's your only hope." << std::endl;
            rooms[player.currentLocationIndex].look();
            currentStoryBeat = StoryBeat::EXPLORING_CENTER;
        }

        void printHelp() {
            std::cout << "\n--- Placeholder Help ---" << std::endl;
            std::cout << "Available commands (Alpha):" << std::endl;
            std::cout << "  goto <location_name> (e.g., goto hall, goto storage)" << std::endl;
            std::cout << "  look                  (examine your current surroundings)" << std::endl;
            std::cout << "  get_all_items         (alpha: quickly get means to leave & surgical item)" << std::endl;
            // Commands for specific story points:
            if (currentStoryBeat == StoryBeat::CHOICE_LEAVE_OR_HELP) {
                std::cout << " choose_leave          (abandon the Guide and flee)" << std::endl;
                std::cout << " choose_help           (try to help the Guide)" << std::endl;
            }
            if (currentStoryBeat == StoryBeat::FINAL_CONFRONTATION_LUNGE) {
                std::cout << "  use_item              (attempt to use the surgical item)" << std::endl;
            }
            std::cout << " help                       (show this help message)" << std::endl;
            std::cout << "  quit                      (exit the game)" << std::endl;
            std::cout << "------------------------------" << std::endl;
        }

        int findRoomIndex(const std::string& simplifiedName) {
            for (size_t i = 0; i < rooms.size(); ++i) {
                if (rooms[i].simplified_name == simplifiedName) {
                    return static_cast<int>(i);
                }
            }
            return -1; // Not found 
        }

        void processInput(const std::string& rawInput) { // Need to finish ("goto" command)
            std::string input = rawInput;
            std::transform(input.begin(), input.end(), input.begin(), ::tolower);

            std::stringstream ss(input);
            std::string command;
            ss >> command;

            if (command == "quit") {
                std::cout << "Exiting game." << std::endl;
                gameOver = true;
                currentStoryBeat = StoryBeat::GAME_OVER;
                return;
            }

            if (gameOver) return; 

            if (command == "help") {
                printHelp();
            } else if (command == "look") {
                rooms[player.currentLocationIndex].look();
                if (player.currentLocationIndex == findRoomIndex("hall")) {
                    std::cout << "[Placeholder] The Guide watches you. He seems to be waiting for you to do something." << std::endl;
                }
            } else if (command == "goto") {
                std::string destinationName;
                ss >> destinationName;
                if (destinationName.empty()) {
                    std::cout << "Goto where? (e.g., goto hall)" << std::endl;
                } else {
                    int destIndex = findRoomIndex(destinationName);
                    if (destIndex != -1) {
                        player.moveTo(destIndex);
                        rooms[player.currentLocationIndex].look();
                        // Simplified item acquisition for Alpha
                        if (rooms[player.currentLocationIndex].simplified_name == "storage" && !player.hasMeansToLeave) {
                            std::cout << "[Placeholder] You rummaged around and found some items that might help fix your car!" << std::endl;
                            player.hasMeansToLeave = true;
                            currentStoryBeat = StoryBeat::FOUND_ALL_MEANS_TO_LEAVE; // Update beat
                            std::cout << "[Placeholder] As you find the last car part, a glint of metal from a nearby shelf catches your eye. It's an odd surgical tool." << std::endl;
                            // SDI becomes available to "get" implicitly with get_all_items or a specific "get surgical_item" later
                        }
                        if (rooms[player.currentLocationIndex].simplified_name == "office" && currentStoryBeat == StoryBeat::PLAYER_SEARCHES_MEDKIT && !player.hasFirstAidKit) {
                            std::cout << "[Placeholder] You found a First Aid Kit in the office!" << std::endl;
                            player.hasFirstAidKit = true;
                        }
                        } else {
                            std::cout << "Cannot find a place called '" << destinationName << "'." << std::endl;
                        }
                }
            } else if (command == "get_all_items") {
                if (!player.hasMeansToLeave || !player.hasSurgicalItem) {
                    std::cout << "[Placeholder] You quickly gather all necessary items to fix the car AND the strange surgical tool you noticed earlier." << std::endl;
                    player.hasMeansToLeave = true;
                    player.hasSurgicalItem = true; // Simplified acquisition
                    currentStoryBeat = StoryBeat::FOUND_ALL_MEANS_TO_LEAVE;
                } else {
                    std::cout << "[Placeholder] You already have these items." << std::endl;
                }
            } else if (command == "choose_leave" && currentStoryBeat == StoryBeat::CHOICE_LEAVE_OR_HELP) {
                std::cout << "[Placeholder] You decide to abandon the Guide and flee." << std::endl;
                currentStoryBeat = StoryBeat::ENDING_NOT_WORTHY;
            } else if (command == "choose_help" && currentStoryBeat == StoryBeat::CHOICE_LEAVE_OR_HELP) {
                std::cout << "[Placeholder] You decide to search for a medkit to help the Guide." << std::endl;
                currentStoryBeat = StoryBeat::PLAYER_SEARCHES_MEDKIT;
                std::cout << "[Placeholder] The Guide weakly gestures... 'Office... please...'" << std::endl;
            } else if (command == "use_item" && currentStoryBeat == StoryBeat::FINAL_CONFRONTATION_LUNGE) {
                if (player.hasSurgicalItem) {
                    std::cout << "[Placeholder] You use the surgical item to defend yourself!" << std::endl;
                    currentStoryBeat = StoryBeat::ENDING_GOOD_ESCAPED;
                } else {
                    std::cout << "[Placeholder] You have nothing to defend yourself with!" << std::endl;
                    currentStoryBeat = StoryBeat::ENDING_BAD_VICTIM;
                }
            } else {
                std::cout << "Unknown command. Type 'help' for options." << std::endl;
            }

        }

        void updateGame() { 
            if (gameOver) return;

            // Story Progression Logic (simplified for Alpha-B)
            switch (currentStoryBeat) {
                case StoryBeat::EXPLORING_CENTER:
                    // Check if player has means to leave and is in a trigger location (e.g., Main Hall)
                    if (player.hasMeansToLeave && rooms[player.currentLocationIndex].simplified_name == "hall") {
                        std::cout << "\n[Placeholder - Guide's Staged Attack - Game Stage 4]" << std::endl;
                        std::cout << "The Guide says: 'You've done well. Almost ready to rush back... But before you go...' He feigns terror, investigates a noise. Horrific sounds follow." << std::endl;
                        std::cout << "You find the Guide appearing gravely injured in the 'Crisis Room' (let's assume you are moved or go there)." << std::endl;
                        // For Alpha, we'll just change the beat. Player should "goto crisis" or be moved.
                        // Let's assume player goes to crisis room next.
                        if (findRoomIndex("crisis") != -1) player.moveTo(findRoomIndex("crisis"));
                        rooms[player.currentLocationIndex].look(); // Show crisis room
                        currentStoryBeat = StoryBeat::GUIDE_STAGES_ATTACK;
                    }
                    break;
                case StoryBeat::FOUND_ALL_MEANS_TO_LEAVE:
                    std::cout << "\n[Placeholder] With the means to leave secured, you feel a sense of urgency. The Guide seems to notice your preparedness." << std::endl;
                    // Transition to EXPLORING_CENTER to allow player to trigger next phase or use get_all_items if they haven't.
                    // The actual trigger for GUIDE_STAGES_ATTACK is having items AND being in the hall.
                    currentStoryBeat = StoryBeat::EXPLORING_CENTER;
                    break;
                case StoryBeat::GUIDE_STAGES_ATTACK:
                    std::cout << "\n[Placeholder] The Guide is 'injured'. Your mind races: 'My Parent... I HAVE to go. But this man...'" << std::endl;
                    std::cout << "What do you do? (choose_leave or choose_help)" << std::endl;
                    currentStoryBeat = StoryBeat::CHOICE_LEAVE_OR_HELP;
                    break;
                case StoryBeat::PLAYER_SEARCHES_MEDKIT:
                    // Player needs to find the medkit. If they have it and return to Guide:
                    if (player.hasFirstAidKit && (rooms[player.currentLocationIndex].simplified_name == "crisis" || rooms[player.currentLocationIndex].simplified_name == "reveal")) {
                        std::cout << "\n[Placeholder - Player returns with Medkit - Game Stage 4]" << std::endl;
                        std::cout << "You return to the Guide with the First Aid Kit..." << std::endl;
                        std::cout << "But he is unharmed, calmly dusting a 'figure'!" << std::endl;
                        currentStoryBeat = StoryBeat::PLAYER_FINDS_GUIDE_UNHARMED_REVEAL;
                    } else if (!player.hasFirstAidKit && rooms[player.currentLocationIndex].simplified_name != "office") {
                        std::cout << "[Placeholder] You need to find the First Aid Kit. The Guide mentioned the 'office'." << std::endl;
                    }
                    break;
                case StoryBeat::PLAYER_FINDS_GUIDE_UNHARMED_REVEAL:
                    std::cout << "\n[Placeholder - Guide's Reveal Monologue - Game Stage 4]" << std::endl;
                    std::cout << "Guide: 'Oh, that old thing? No, my dear friend... You chose to stay... Such purity...'" << std::endl;
                    std::cout << "(He reveals the 'figures' are taxidermied humans, his 'collection'.)" << std::endl;
                    std::cout << "Guide: 'It is time for you to join them.' He lunges!" << std::endl;
                    currentStoryBeat = StoryBeat::FINAL_CONFRONTATION_LUNGE;
                    break;
                case StoryBeat::FINAL_CONFRONTATION_LUNGE:
                    std::cout << "\n[Placeholder - Final Confrontation - Game Stage 5]" << std::endl;
                    std::cout << "The Guide is attacking! What do you do? (use_item or other failed action)" << std::endl;
                    // Input will determine next state (ENDING_GOOD or ENDING_BAD)
                    break;
                case StoryBeat::ENDING_NOT_WORTHY:
                    std::cout << "\n--- ENDING 1: Not Worthy ---" << std::endl;
                    std::cout << "[Placeholder Text] As you depart, the Guide's chilling remark: 'Go then... Some souls... simply aren't worth preserving.'" << std::endl;
                    std::cout << "You escape, the words a brand, but the drive to the hospital consuming all else." << std::endl;
                    gameOver = true;
                    currentStoryBeat = StoryBeat::GAME_OVER;
                    break;
                case StoryBeat::ENDING_GOOD_ESCAPED:
                    std::cout << "\n--- ENDING 2: Good Ending - Escaped ---" << std::endl;
                    std::cout << "[Placeholder Text] You managed to use the item and fend off the Guide!" << std::endl;
                    std::cout << "You flee the horrific visitor center, forever scarred, but with a desperate hope for your parent." << std::endl;
                    gameOver = true;
                    currentStoryBeat = StoryBeat::GAME_OVER;
                    break;
                case StoryBeat::ENDING_BAD_VICTIM:
                    std::cout << "\n--- ENDING 3: Bad Ending - Victim ---" << std::endl;
                    std::cout << "[Placeholder Text] His smile was the last thing you saw... 'Welcome,' he whispered, 'to eternity.'" << std::endl;
                    std::cout << "The Oakhaven Visitor Center has claimed another exhibit." << std::endl;
                    gameOver = true;
                    currentStoryBeat = StoryBeat::GAME_OVER;
                    break;
                case StoryBeat::INTRO: // Should be handled by displayIntro
                case StoryBeat::CHOICE_LEAVE_OR_HELP: // Waits for player input
                case StoryBeat::GAME_OVER: // Game has ended
                    break;
            }
        }

        void run() {
            displayIntro();
            std::string inputLine;

            while (!gameOver) {
                std::cout << "\n> ";
                if (!std::getline(std::cin, inputLine)) {
                    break;
                }
                processInput(inputLine);
                updateGame();
            }

            std::cout << "\n--- Thank you for playing The Visitor Center (Alpha!) ---" << std::endl;
        }
};

 int main() {
    Game visitorCenterGame;
    visitorCenterGame.run();
    return 0;
 }