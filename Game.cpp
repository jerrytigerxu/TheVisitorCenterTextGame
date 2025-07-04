#include "Game.h"
#include <iostream>
#include <algorithm>

// Constructor
Game::Game()
    : player(nullptr), // Player needs a starting room, will be set in setupGame
    guide("The Visitor Guide"),
    currentGameState(GameState::INTRO),
    gameOver(false),
    surgicalItemSpawned(false) {
        setupGame();
}

// Initializes game objects
void Game::setupGame() {
    setupRoomsAndExits();
    setupItems();
    setupInteractiveElements();
    // Player needs a stating room, find it after rooms are created
    Room* startRoom = findRoomById("car_breakdown");
    if (!startRoom) {
        allRooms.push_back(std::make_unique<Room>("default_start", "Default Start Room", "Something went wrong, starting in a default room."));
        startRoom = allRooms[0].get();
        std::cerr << "Error: Start room 'car_breakdown' not found. Using default." << std::endl;
    }
    player.currentLocation = startRoom; // Initialize player's location

    setupGuide();

    // Initial look for the player
    if (player.currentLocation) {
        player.currentLocation->look();
    }
}

void Game::setupRoomsAndExits() {
    // Create rooms
    allRooms.push_back(std::make_unique<Room>("car_breakdown", "Car Breakdown Site", "Your car has sputtered to a halt beside a desolate road. The imposing Oakhaven Visitor Center is your only visible shelter."));
    allRooms.push_back(std::make_unique<Room>("vc_entrance", "Visitor Center Entrance", "You stand at the threshold of the Oakhaven Visitor Center. The air is unnervingly still, and shadows cast by the setting sun seem to twist and writhe at the edges of your vision. It feels less like a building and more like a tomb holding its breath."));
    allRooms.push_back(std::make_unique<Room>("main_hall", "Main Hall", "A large, dusty main hall stretches before you. Lifelike figures stand in silent watch from shadowy alcoves. An older man, the Guide, is here. He eyes you curiously."));
    allRooms.push_back(std::make_unique<Room>("storage_room", "Storage Room", "A cluttered storage area, filled with forgotten supplies and cobwebs. It smells of dust and decay."));
    allRooms.push_back(std::make_unique<Room>("office", "Office", "An old, neglected office. A large wooden desk sits in the center, covered in yellowed papers. There's a filing cabinet in the corner."));
    allRooms.push_back(std::make_unique<Room>("crisis_room", "West Wing Corridor", "A dim corridor in what seems to be a less-used part of the center. The Guide mentioned investigating a noise from this direction. It feels colder here.")); 
    allRooms.push_back(std::make_unique<Room>("reveal_spot", "Main Hall - Collection Display", "You are back in the Main Hall. The Guide stands near one of the alcoves, a strange calm about him. The figures seem more prominent now."));

    // Get raw pointers for exits 
    Room* car = findRoomById("car_breakdown");
    Room* entrance = findRoomById("vc_entrance");
    Room* hall = findRoomById("main_hall");
    Room* storage = findRoomById("storage_room");
    Room* office = findRoomById("office");
    Room* crisis = findRoomById("crisis_room"); 

    // Define exits with hyphenated keys
    if (car && entrance) car->addExit("enter-center", entrance); 
    if (entrance && hall && car) {
        entrance->addExit("enter", hall); // "enter" can remain if it's a single word action into the hall
        entrance->addExit("leave-center", car); // Or just "leave" if context is clear
    }
    if (hall && storage && office && crisis && entrance) {
        hall->addExit("storage", storage); // Assuming "storage" is a direct, single-word exit
        hall->addExit("office", office);   // Assuming "office" is a direct, single-word exit
        hall->addExit("west-wing", crisis); 
        hall->addExit("exit-center", entrance);
    }
    if (storage && hall) storage->addExit("hall", hall);
    if (office && hall) office->addExit("hall", hall);
    if (crisis && hall) crisis->addExit("hall", hall); 
}

void Game::setupItems() {
    // "Means to leave" items - Placed for sequential pickup
    // Item 1: Gas Can (Storage Room)
    Room* storage = findRoomById("storage_room");
    if (storage) {
        storage->addItem(std::make_unique<Item>("gas_can", "Gas Can", "A red, slightly rusted gas can. It feels like it has some fuel in it. This looks like the first thing you'll need."));
        // Item 2: Spare Tire (Storage Room - will be gettable after Gas Can)
        storage->addItem(std::make_unique<Item>("spare_tire", "Spare Tire", "A dusty but seemingly usable spare tire."));
    }

    // Item 3: Oil Fluid (Office - will be gettable after Spare Tire)
    // This is also the room where the surgical_item will appear.
    Room* office = findRoomById("office");
    if (office) {
        office->addItem(std::make_unique<Item>("oil_fluid", "Oil Fluid", "A sealed container of motor oil. The last piece of the puzzle for the car."));
        // First Aid Kit - available without sequence
        office->addItem(std::make_unique<Item>("first_aid_kit", "First Aid Kit", "A standard first aid kit. Looks relatively well-stocked."));
    }

    // Surgical Defensive Item is NOT placed here initially.
    // It will be dynamically added to the "office" when "oil_fluid" is picked up.
}

void Game::setupInteractiveElements() {
    Room* hall = findRoomById("main_hall");
    if (hall) {
        hall->addInteractiveElement(InteractiveElement("figures", 
            "The 'exhibits' are figures depicting scenes from Oakhaven's history. From a distance, they look like wax, but up close, the detail is unnerving. The texture of the skin is too porous, the hair seems too fine, and the eyes have a glassy, wet-looking sheen that makes you want to look away.",
            "The 'figures' are no exhibits. They are horrifyingly preserved human bodies, skin like leather, eyes fixed in a moment of past terror. The Guide's 'collection'."));
        hall->addInteractiveElement(InteractiveElement("guide", 
            "The Visitor Guide is an older man, with eyes that dart nervously around the room. He carries the weight of this place on his shoulders, an air of profound fear about him.",
            "The Guide's fear is gone, replaced by a triumphant, predatory smile. He is the master of this macabre gallery, the hunter who has successfully lured his prey."));
    }
     Room* office = findRoomById("office");
    if (office) {
        office->addInteractiveElement(InteractiveElement("desk", "A large wooden desk covered in yellowed papers and dust. A few drawers are visible.", "The papers on the desk are old visitor logs, some entries ending abruptly."));
        office->addInteractiveElement(InteractiveElement("filing_cabinet", "A tall, metal filing cabinet. It looks locked.", "The filing cabinet might hold records, but it's securely locked."));
    }
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
    transitionToState(GameState::AWAITING_TASK_1);
}

void Game::transitionToState(GameState newState) {
    // Optional: Add logic here if certain transitions trigger immediate effects or messages
    // std::cout << "[Debug] Transitioning from " << static_cast<int>(currentGameState) << " to " << static_cast<int>(newState) << std::endl;
    currentGameState = newState;

    // Specific actions on entering a new state
    switch (newState) {
        case GameState::GUIDE_FACES_VENGEANCE:
            guide.talk(currentGameState); // Guide screams his "desecrated" line
            // After this, sounds of an attack should begin. This will lead to the choice.
            // For now, we transition directly to the choice point.
            transitionToState(GameState::CHOICE_POINT_LEAVE_OR_HELP);
            break;
        case GameState::CHOICE_POINT_LEAVE_OR_HELP:
             std::cout << "\nWith the sounds of the attack echoing behind you, your mind races. Your mother... waiting." << std::endl;
             std::cout << "But you caused this. You doomed him. What kind of person would you be if you just left?" << std::endl;
             std::cout << "Your choices: 'leave' or 'help'." << std::endl;
            break;
        case GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL:
            guide.setFeigningInjury(false); // No longer needed but good practice
            guide.talk(currentGameState); 
            transitionToState(GameState::FIGURES_REVEALED);
            break;
        case GameState::FIGURES_REVEALED:
            if (player.currentLocation) {
                InteractiveElement* figures = player.currentLocation->getInteractiveElement("figures");
                if (figures) figures->reveal();
            }
            guide.talk(currentGameState); 
            transitionToState(GameState::FINAL_CONFRONTATION_IMMINENT);
            break;
        case GameState::FINAL_CONFRONTATION_IMMINENT:
             guide.talk(currentGameState); 
             std::cout << "\nHe lunges towards you!" << std::endl;
            break;
        case GameState::ENDING_NOT_WORTHY:
        case GameState::ENDING_GOOD_ESCAPED:
        case GameState::ENDING_BAD_VICTIM:
            displayEnding(currentGameState);
            break;
        default:
            // For states like AWAITING_TASK_1, the Guide's dialogue is triggered by `talk to guide`,
            // so no special transition effect is needed here.
            break;
    }
}

void Game::displayEnding(GameState endingType) {
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
            std::cout << "With a desperate surge of adrenaline, you use the " << (player.getItemFromInventory("surgical_item") ? player.getItemFromInventory("surgical_item")->id : "item") << "!" << std::endl;
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

        if (!gameOver) {
            std::cout << "\n==================================================================\n\n";
        }

        if (!gameOver) {
            if (player.currentLocation) {
                std::cout << "[" << player.currentLocation->name <<"] > ";
            } else {
                std::cout << "[Unknown location] > ";
            }
        }

    }

    if (currentGameState != GameState::GAME_OVER) {
        std::cout << "\n--- Game ended abruptly. ---" << std::endl;
    }
    std::cout << "\n--- Thank you for playing The Visitor Center! ---" << std::endl;
}

std::vector<std::string> Game::parseCommand(const std::string& rawInput) {
    std::vector<std::string> words;
    std::string input = rawInput;
    // Convert to lowercase
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    std::stringstream ss(input);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// Processes player input 
void Game::processInput(const std::string& rawInput) {
    if (gameOver) return;

    std::vector<std::string> words = parseCommand(rawInput);
    if (words.empty()) {
        return;
    }

    std::string command = words[0];

    // Handle single-action final confrontation
    if (currentGameState == GameState::FINAL_CONFRONTATION_IMMINENT) {
        if (command == "use" && words.size() > 1 && words[1] == "surgical_item") {
            if (player.hasItem("surgical_item")) {
                transitionToState(GameState::ENDING_GOOD_ESCAPED);
            } else {
                std::cout << "You don't have that!" << std::endl;
                transitionToState(GameState::ENDING_BAD_VICTIM);
            }
        } else {
            std::cout << "You panic. Your action is ineffective as the Guide overwhelms you." << std::endl;
            transitionToState(GameState::ENDING_BAD_VICTIM);
        }
        return; // Stop further processing
    }

    // General commands

    if (command == "quit") {
        std::cout << "Exiting game." << std::endl;
        gameOver = true;
        currentGameState = GameState::GAME_OVER;
    } else if (command == "go" || command == "move") {
        handleGoCommand(words);
    } else if (command == "look" || command == "l") {
        handleLookCommand(words);
    } else if (command == "examine" || command == "x" || command == "inspect") {
        handleExamineCommand(words);
    } else if (command == "get" || command == "take" || command == "pickup") {
        handleGetCommand(words);
    } else if (command == "inventory" || command == "i" || command == "inv") {
        handleInventoryCommand(words);
    } else if (command == "talk") {
        handleTalkCommand(words);
    } else if (command == "help" || command == "?") {
        handleHelpCommand(words);
    } else if (command == "use") {
        handleUseCommand(words);
    } else if (command == "choose" && (currentGameState == GameState::CHOICE_POINT_LEAVE_OR_HELP)) {
        handleChooseCommand(words);
    }
    // Developer / Debug commands
    else if (command == "dbg_setstate" && words.size() > 1) { // Debug command
        try {
            int state_val = std::stoi(words[1]);
            transitionToState(static_cast<GameState>(state_val));
            std::cout << "[Debug] State set to " << words[1] << std::endl;
        } catch (const std::exception& e) {
            std::cout << "[Debug] Invalid state value." << std::endl;
        }
    } else if (command == "dbg_playeritems"){ // Debug command
        player.hasGasCan = player.hasSpareTire = player.hasOilFluid = true;
        std::cout << "[Debug] Player has all means to leave." << std::endl;
        if (!player.hasItem("surgical_item")) { // Give surgical item if not present
            player.pickUpItem(std::make_unique<Item>("surgical_item", "Surgical Instrument (Debug)", "A debug surgical tool."));
        }
         if (!player.hasItem("first_aid_kit")) { 
            player.pickUpItem(std::make_unique<Item>("first_aid_kit", "First Aid Kit (Debug)", "A debug first aid kit."));
        }
        player.showInventory();
    }
     else {
        std::cout << "Unknown command. Type 'help' for options." << std::endl;
    }
}

// Command handlers 
void Game::handleGoCommand(const std::vector<std::string>& words) {
    if (words.size() < 2) {
        std::cout << "Go where?" << std::endl;
        return;
    }
    std::string destination_key = words[1];
    if (player.currentLocation && player.currentLocation->exits.count(destination_key)) {
        Room* nextRoom = player.currentLocation->exits[destination_key];
        player.moveTo(nextRoom);
        // *** FIXED: Removed obsolete logic referencing old states ***
        if (nextRoom && nextRoom->id == "main_hall" && currentGameState == GameState::PLAYER_FOUND_MEDKIT) {
             transitionToState(GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL);
        }
    } else {
        std::cout << "You can't go '" << destination_key << "' from here." << std::endl;
    }
}

void Game::handleLookCommand([[maybe_unused]] const std::vector<std::string>& words) {
    if (player.currentLocation) {
        player.currentLocation->look();
        if (player.currentLocation->id == "main_hall" && currentGameState <= GameState::AWAITING_TASK_3) {
            std::cout << "The Guide watches you, a faint, unreadable expression on his face." << std::endl;
        }
    } else {
        std::cout << "You are nowhere in particular. This is odd." << std::endl;
    }
}

void Game::handleExamineCommand(const std::vector<std::string>& words) {
    if (words.size() < 2) {
        std::cout << "Examine what?" << std::endl;
        return;
    }
    std::string targetName = words[1];
    // If command was "examine the desk", targetName might be "the". Then actual target is words[2]
     if (targetName == "the" && words.size() > 2) {
        targetName = words[2];
    }


    // Check room's items
    if (player.currentLocation) {
        Item* roomItem = player.currentLocation->getItem(targetName);
        if (roomItem) {
            roomItem->examine();
            return;
        }
        // Check room's interactive elements
        InteractiveElement* element = player.currentLocation->getInteractiveElement(targetName);
        if (element) {
            element->examine();
            // Special case: examining guide
            if (element->name == "guide_himself") {
                 guide.talk(currentGameState); // Guide might say something when examined
            }
            return;
        }
    }
    // Check player's inventory
    Item* invItem = player.getItemFromInventory(targetName);
    if (invItem) {
        invItem->examine();
        return;
    }
    // Check if examining the Guide directly by name
    if (targetName == "guide" || targetName == guide.name) { // guide.name might need tolower
         InteractiveElement* guideElement = player.currentLocation->getInteractiveElement("guide_himself");
         if(guideElement) guideElement->examine();
         else std::cout << "The Guide is present. " << guide.name << " watches you." << std::endl;
         guide.talk(currentGameState);
        return;
    }

    std::cout << "You don't see any '" << targetName << "' here to examine, nor are you carrying it." << std::endl;
}

void Game::handleGetCommand(const std::vector<std::string>& words) {
    if (words.size() < 2) { std::cout << "Get what?" << std::endl; return; }
    std::string itemId = words[1];

    if (player.currentLocation && player.currentLocation->getItem(itemId)) {
        // *** FIXED: Removed all state transition logic from getting items. ***
        // This will be handled by `use` commands for tasks now.
        std::unique_ptr<Item> item = player.currentLocation->removeItem(itemId);
        
        // Special logic for surgical item spawning
        if (item->id == "oil_fluid" && !surgicalItemSpawned) {
             Room* officeRoom = findRoomById("office");
             if(officeRoom) {
                officeRoom->addItem(std::make_unique<Item>("surgical_item", "Surgical Instrument", "An antique surgical instrument, surprisingly well-maintained. It was tucked away near where the oil was. Almost... waiting."));
                surgicalItemSpawned = true;
                std::cout << "\nAs you pick up the oil, a glint of metal from a shadowy corner catches your eye." << std::endl;
             }
        }
        
        // Logic for returning to guide with medkit
        if(item->id == "first_aid_kit" && currentGameState == GameState::CHOICE_POINT_LEAVE_OR_HELP) {
            transitionToState(GameState::PLAYER_FOUND_MEDKIT);
             std::cout << "You have the First Aid Kit. You should return to the Guide in the main hall." << std::endl;
        }

        player.pickUpItem(std::move(item));
    } else {
        std::cout << "You don't see any '" << itemId << "' here." << std::endl;
    }
}

void Game::handleInventoryCommand([[maybe_unused]] const std::vector<std::string>& words) {
    player.showInventory();
}

void Game::handleTalkCommand(const std::vector<std::string>& words) {
    if (words.size() > 1 && (words[1] == "to" || words[1] == "with") && words.size() > 2 && words[2] == "guide") {
        if (player.currentLocation && player.currentLocation->id == "main_hall") { // Or wherever guide is
            guide.talk(currentGameState);
        } else {
            std::cout << "The Guide is not here." << std::endl;
        }
    } else if (words.size() > 1 && words[1] == "guide") {
         if (player.currentLocation && player.currentLocation->id == "main_hall") {
            guide.talk(currentGameState);
        } else {
            std::cout << "The Guide is not here." << std::endl;
        }
    }
    else {
        std::cout << "Talk to whom? (e.g., 'talk to guide')" << std::endl;
    }
}

void Game::handleHelpCommand(const std::vector<std::string>& words) {
    if (words.size() > 1) {
        guide.provideHelp(words[1], currentGameState);
    } else {
        guide.provideHelp("general", currentGameState);
    }
}

void Game::handleUseCommand(const std::vector<std::string>& words) {
    if (words.size() < 2) { std::cout << "Use what?" << std::endl; return; }
    std::string itemId = words[1];

    if (!player.hasItem(itemId)) {
        std::cout << "You don't have a '" << itemId << "' to use." << std::endl;
        return;
    }
    
    // *** NOTE: This is where task completion logic will go. ***
    // For example: `if (itemId == "cloth" && currentGameState == AWAITING_TASK_1)`
    // For now, it will just give a generic message.
    
    std::cout << "You try to use the " << itemId << ", but nothing specific happens." << std::endl;
}

void Game::handleChooseCommand(const std::vector<std::string>& words) {
    if (currentGameState != GameState::CHOICE_POINT_LEAVE_OR_HELP) {
        std::cout << "There's no specific choice to make right now with that command." << std::endl;
        return;
    }
    if (words.size() < 2) {
        std::cout << "Choose what? ('choose leave' or 'choose help')" << std::endl;
        return;
    }
    std::string choice = words[1];
    if (choice == "leave") {
        transitionToState(GameState::ENDING_NOT_WORTHY);
    } else if (choice == "help") {
        std::cout << "You decide to help the Guide. He weakly gestures towards the office, muttering about a First Aid Kit." << std::endl;
    } else {
        std::cout << "That's not a valid choice here. Try 'choose leave' or 'choose help'." << std::endl;
    }
}

// Updates game state
void Game::updateGame() {
    if (gameOver) return;

    // This function will be used later to trigger timed events or passive state changes.
}