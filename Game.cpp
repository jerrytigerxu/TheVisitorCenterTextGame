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
    allRooms.push_back(std::make_unique<Room>("vc_entrance", "Visitor Center Entrance", "You stand at the threshold of the Oakhaven Visitor Center. The air is cold and still. A heavy wooden door leads inside."));
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
    // "Means to leave"
    Room* storage = findRoomById("storage_room");
    if (storage) {
        storage->addItem(std::make_unique<Item>("gas_can", "Gas Can", "A red, slightly rusted gas can. It feels like it has some fuel in it."));
        storage->addItem(std::make_unique<Item>("spare_tire", "Spare Tire", "A dusty but seemingly usable spare tire."));
    }
    Room* office = findRoomById("office");
    if (office) {
        office->addItem(std::make_unique<Item>("oil_fluid", "Oil Fluid", "A sealed container of motor oil."));
        // First Aid Kit
        office->addItem(std::make_unique<Item>("first_aid_kit", "First Aid Kit", "A standard first aid kit. Looks relatively well-stocked."));
    }

    // Surgical Defensive Item - its placement is conditional (after finding last "means to leave")
    // For Beta A, we can place it somewhere and make its "get" logic conditional later, or place it once triggered.
    // Let's place it in storage for now, to be made noticeable later.
    if (storage) {
         storage->addItem(std::make_unique<Item>("surgical_item", "Surgical Instrument", "An antique surgical instrument, surprisingly well-maintained. Out of place. Almost... waiting."));
    }
}

void Game::setupInteractiveElements() {
    Room* hall = findRoomById("main_hall");
    if (hall) {
        hall->addInteractiveElement(InteractiveElement("wax_figures_alcove1", 
            "Dusty wax figures stand in an alcove, depicting a pioneer family. Their eyes seem to follow you.",
            "The 'pioneer family' are no wax figures. They are horrifyingly preserved human bodies, skin like leather, eyes fixed in a moment of past terror."));
        hall->addInteractiveElement(InteractiveElement("wax_figures_alcove2", 
            "Another alcove contains figures of local historical personalities. They are unnervingly still.",
            "These too are human remains, twisted into grotesque parodies of life. The Guide's 'collection'."));
        hall->addInteractiveElement(InteractiveElement("guide_himself", // Player can examine the guide
            "The Visitor Guide is an older man, with an air of unsettling calm. His eyes are sharp and observant.",
            "The Guide's smile is no longer benign. It's predatory, triumphant. He is the master of this macabre gallery."));
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
    transitionToState(GameState::EXPLORING_CENTER_INITIAL);
}

void Game::transitionToState(GameState newState) {
    // Optional: Add logic here if certain transitions trigger immediate effects or messages
    // std::cout << "[Debug] Transitioning from " << static_cast<int>(currentGameState) << " to " << static_cast<int>(newState) << std::endl;
    currentGameState = newState;

    // Specific actions on entering a new state
    switch (newState) {
        case GameState::GUIDE_STAGES_ATTACK_PRELUDE:
            std::cout << "\n" << guide.name << ": \"You've done well. Almost ready to rush back to your mother’s side. But before you go...\"" << std::endl;
            std::cout << "He trails off, his head snapping towards the West Wing corridor. \"What was that noise?\"" << std::endl;
            std::cout << "He bravely (or foolishly?) heads towards the " << findRoomById("crisis_room")->name << " to investigate." << std::endl;
            // Player might need to follow, or game auto-moves Guide. For now, player needs to go there.
            // Guide is now "in" the crisis room conceptually for the next stage.
            transitionToState(GameState::GUIDE_STAGES_ATTACK_SOUNDS);
            break;
        case GameState::GUIDE_STAGES_ATTACK_SOUNDS:
            std::cout << "\nA moment later, horrific sounds echo from the " << findRoomById("crisis_room")->name << " – a choked scream, a sickening thud." << std::endl;
            std::cout << "Silence." << std::endl;
            // Player needs to go to crisis_room to find the guide.
            // The state will change to GUIDE_FEIGNING_INJURY when player enters crisis_room and guide is there.
            break;
        case GameState::GUIDE_FEIGNING_INJURY:
            guide.setFeigningInjury(true);
            std::cout << "\nYou rush into the " << player.currentLocation->name << " and find the Guide slumped against a wall, appearing gravely injured. A dark stain is spreading on his shirt." << std::endl;
            guide.talk(currentGameState); // Guide says his feigning injury lines
            transitionToState(GameState::CHOICE_POINT_LEAVE_OR_HELP);
            break;
        case GameState::CHOICE_POINT_LEAVE_OR_HELP:
            std::cout << "\nYour mind races. Your mother... in the hospital... waiting, maybe scared. You HAVE to go." << std::endl;
            std::cout << "But this man... he’s hurt. What kind of person would you be if you just left him here to die, after he (supposedly) tried to help you?" << std::endl;
            std::cout << "What would your mother think of you?" << std::endl;
            std::cout << "Your choices: 'choose leave' or 'choose help'." << std::endl;
            break;
        case GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL:
            guide.setFeigningInjury(false); // Guide is no longer feigning
            std::cout << "\nYou return to where you left the Guide, First Aid Kit in hand..." << std::endl;
            std::cout << "But he is unharmed, calmly dusting one of the 'figures' in an alcove." << std::endl;
            // Player might have dropped items, Guide hides them
            // (This logic needs to be more fleshed out: where were items dropped? How does Guide get them?)
            std::cout << "You notice the car parts you gathered are gone from where you left them!" << std::endl; 
            // For Beta A, just a statement. Player's inventory is not cleared yet by this.
            // Explicitly deliver all dialogue lines for this state
            std::cout << "\n--- " << guide.name << " ---" << std::endl;
            if (guide.dialogueLines.count(currentGameState) && !guide.dialogueLines[currentGameState].empty()) {
                for (const auto& line : guide.dialogueLines[currentGameState]) {
                    std::cout << "\"" << line << "\"" << std::endl;
                }
            } else {
                // Fallback if no specific lines are found (though they are defined in Guide.cpp)
                std::cout << "\"" << "The Guide regards you silently for a moment." << "\"" << std::endl;
            }
            std::cout << "--------------------------------" << std::endl;
            //guide.talk(currentGameState); // Guide starts his monologue
            transitionToState(GameState::FIGURES_REVEALED);
            break;
        case GameState::FIGURES_REVEALED:
            std::cout << "\nThe horrifying truth dawns on you. The 'exhibits' are his collection..." << std::endl;
            if (player.currentLocation) {
                for(auto& element_ptr : player.currentLocation->interactive_elements){
                    if(element_ptr.name.find("wax_figures") != std::string::npos){
                        element_ptr.reveal();
                    }
                }
            }
            guide.talk(currentGameState); // Continues monologue or prepares for lunge
            transitionToState(GameState::FINAL_CONFRONTATION_IMMINENT);
            break;
        case GameState::FINAL_CONFRONTATION_IMMINENT:
             guide.talk(currentGameState); // Guide says his "time to join them" lines
             std::cout << "\nHe lunges towards you!" << std::endl;
             std::cout << "What do you do? (e.g., 'use surgical_item')" << std::endl;
            break;
        // Endings
        case GameState::ENDING_NOT_WORTHY:
        case GameState::ENDING_GOOD_ESCAPED:
        case GameState::ENDING_BAD_VICTIM:
            displayEnding(currentGameState);
            break;
        default:
            // No specific action for other states on transition by default
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

    if (currentGameState == GameState::FINAL_CONFRONTATION_IMMINENT) {
        std::string command = words[0]; // Ensure command is defined from words
        std::string itemToUse = "";
        if (words.size() > 1) {
            itemToUse = words[1];
            if (itemToUse == "the" && words.size() > 2) { // Handle "use the item"
                itemToUse = words[2];
            }
        }

        if (command == "use" && itemToUse == "surgical_item") {
            if (player.hasItem("surgical_item")) {
                // Correct action. Let it fall through to the main command processing
                // which will call handleUseCommand, leading to ENDING_GOOD_ESCAPED.
            } else {
                std::cout << "You desperately reach for the " << itemToUse << ", but realize you don't have it!" << std::endl;
                transitionToState(GameState::ENDING_BAD_VICTIM);
                return; // Game ends here, stop further processing for this input
            }
        } else {
            // Any other command, or using a different item, or trying to use an item without "use"
            if (command == "use") { // Tried to use something, but not the correct surgical item
                 std::cout << "You attempt to use the " << itemToUse << ", but it's useless against the Guide's advance!" << std::endl;
            } else { // Typed something else like "look", "go", or an unknown command
                std::cout << "You panic. Your action '" << rawInput << "' is ineffective as the Guide overwhelms you." << std::endl;
            }
            transitionToState(GameState::ENDING_BAD_VICTIM);
            return; // Game ends here, stop further processing for this input
        }
    }

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
        std::cout << "Go where? (e.g., 'go north', 'go office', 'go enter-center')" << std::endl;
        return;
    }
    // The destination is now expected to be a single argument (words[1]),
    // which could be a simple direction or a hyphenated/underscored phrase.
    std::string destination_key = words[1]; 

    // Optional: if you still want to allow "go to <destination>", you can check for "to"
    // and use words[2] if "to" is present as words[1].
    // For strict hyphenated/underscored single argument, this is not needed.
    // if (words.size() > 2 && words[1] == "to") {
    //     destination_key = words[2];
    // }


    if (player.currentLocation && player.currentLocation->exits.count(destination_key)) {
        Room* nextRoom = player.currentLocation->exits[destination_key];
        player.moveTo(nextRoom); 

        if (nextRoom) { 
            if (nextRoom->id == "crisis_room" && currentGameState == GameState::GUIDE_STAGES_ATTACK_SOUNDS) {
                transitionToState(GameState::GUIDE_FEIGNING_INJURY);
            } else if ((nextRoom->id == "main_hall" || nextRoom->id == "reveal_spot") && currentGameState == GameState::PLAYER_FOUND_MEDKIT) {
                transitionToState(GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL);
            }
        }
    } else {
        std::cout << "You can't go '" << destination_key << "' from here, or it's not a known exit." << std::endl;
    }
}

void Game::handleLookCommand([[maybe_unused]] const std::vector<std::string>& words) {
    if (player.currentLocation) {
        player.currentLocation->look();
        if (player.currentLocation->id == "main_hall" && 
            (currentGameState == GameState::EXPLORING_CENTER_INITIAL || currentGameState == GameState::PLAYER_HAS_SOME_MEANS)) {
            std::cout << "The Guide watches you, a faint, unreadable smile on his lips." << std::endl;
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
    if (words.size() < 2) {
        std::cout << "Get what?" << std::endl;
        return;
    }
    std::string itemId = words[1];
    if (itemId == "the" && words.size() > 2) { // "get the gas_can"
        itemId = words[2];
    }

    // Special condition for surgical_item:
    // Player should only be able to get it if they have all other means to leave,
    // as the narrative cue for it being "noticeable" happens then.
    if (itemId == "surgical_item" && !player.hasAllMeansToLeave()) {
        // Check if the item is even in the room to give a more specific message
        if (player.currentLocation && player.currentLocation->getItem(itemId)) {
            std::cout << "You see the " << itemId << ", but it doesn't seem particularly useful or important to you right now." << std::endl;
        } else {
            std::cout << "You don't see any '" << itemId << "' here." << std::endl; // Or a generic "cannot get"
        }
        return;
    }

    if (player.currentLocation) {
        std::unique_ptr<Item> item = player.currentLocation->removeItem(itemId);
        if (item) {
            player.pickUpItem(std::move(item)); // pickUpItem also updates player flags

            bool justReachedAllMeansState = false;

            // Check for game state changes after picking up items
            if (player.hasAllMeansToLeave()) {
                if (currentGameState < GameState::PLAYER_HAS_ALL_MEANS) {
                    transitionToState(GameState::PLAYER_HAS_ALL_MEANS);
                    justReachedAllMeansState = true; // Set the flag
                }
            } else if ((player.hasGasCan || player.hasSpareTire || player.hasOilFluid) && currentGameState < GameState::PLAYER_HAS_SOME_MEANS) {
                 transitionToState(GameState::PLAYER_HAS_SOME_MEANS);
            }

            // If we *just* transitioned to PLAYER_HAS_ALL_MEANS, show the surgical item notice
            if (justReachedAllMeansState) {
                Item* surgicalItemInRoom = nullptr; // Check for the item in its typical location
                Room* storage = findRoomById("storage_room");
                if (storage) {
                    surgicalItemInRoom = storage->getItem("surgical_item");
                }

                if (surgicalItemInRoom) { // If the item is still in the storage room (i.e., not picked up yet)
                   std::cout << "\nAs you consider the last part for your car, a glint of metal from a shelf in the "
                             << (storage ? storage->name : "storage area")
                             << " catches your eye again, or perhaps you recall seeing it more clearly now. It's an odd "
                             << surgicalItemInRoom->id << "." << std::endl;
                }
            }
            
            // If player picked up first aid kit during specific state
            if (itemId == "first_aid_kit" && currentGameState == GameState::PLAYER_CHOOSES_HELP_SEARCH_MEDKIT) {
                transitionToState(GameState::PLAYER_FOUND_MEDKIT);
                std::cout << "You have the First Aid Kit. You should return to the Guide." << std::endl;
            }

        } else {
            std::cout << "You don't see any '" << itemId << "' here to get." << std::endl;
        }
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
    if (words.size() < 2) {
        std::cout << "Use what?" << std::endl;
        return;
    }
    std::string itemId = words[1];
    // If command was "use the item_id", itemId might be "the". Then actual item_id is words[2]
     if (itemId == "the" && words.size() > 2) {
        itemId = words[2];
    }

    if (!player.hasItem(itemId)) {
        std::cout << "You don't have a '" << itemId << "' to use." << std::endl;
        return;
    }

    // Specific item usage logic
    if (itemId == "surgical_item" && currentGameState == GameState::FINAL_CONFRONTATION_IMMINENT) {
        std::cout << "You brandish the " << player.getItemFromInventory(itemId)->id << " defensively!" << std::endl;
        transitionToState(GameState::ENDING_GOOD_ESCAPED);
    } else if (itemId == "first_aid_kit") {
        if (currentGameState == GameState::GUIDE_FEIGNING_INJURY || currentGameState == GameState::CHOICE_POINT_LEAVE_OR_HELP) {
             std::cout << "You try to use the First Aid Kit on the Guide..." << std::endl;
             // This action might be part of the "choose_help" path implicitly,
             // or if player tries to use it directly on him.
             // The reveal happens when player *returns* with it.
             std::cout << "He seems to need it, but you should probably focus on getting it to him if you chose to help." << std::endl;

        } else {
             std::cout << "You patch yourself up a bit with the " << player.getItemFromInventory(itemId)->id << ". You feel slightly better, though the dread remains." << std::endl;
        }
    }
    // Add more item usage cases here
    else {
        Item* itemToUse = player.getItemFromInventory(itemId);
        if(itemToUse) itemToUse->use(); // Generic use message
    }
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
        transitionToState(GameState::PLAYER_CHOOSES_HELP_SEARCH_MEDKIT);
    } else {
        std::cout << "That's not a valid choice here. Try 'choose leave' or 'choose help'." << std::endl;
    }
}

// Updates game state
void Game::updateGame() {
    if (gameOver) return;

    // This function will house logic that triggers based on game state,
    // player conditions, etc., that isn't a direct result of a single command.
    // For Beta A, most state transitions are handled within command handlers or transitionToState.

    // Example: If player has all means to leave and is in main hall with guide, trigger attack prelude
    if (currentGameState == GameState::PLAYER_HAS_ALL_MEANS &&
        player.currentLocation && player.currentLocation->id == "main_hall") {
        // Check if Guide is present (implicitly yes for main_hall in this design)
        transitionToState(GameState::GUIDE_STAGES_ATTACK_PRELUDE);
    }
    
    // If player chose to help and is in office without medkit, remind them.
    if (currentGameState == GameState::PLAYER_CHOOSES_HELP_SEARCH_MEDKIT &&
        player.currentLocation && player.currentLocation->id == "office" && !player.hasFirstAidKit) {
        std::cout << "You are in the office. The Guide mentioned a First Aid Kit might be here." << std::endl;
    }


    // If player has surgical item and is in final confrontation but hasn't used it (input handles this)
    // If player doesn't have surgical item in final confrontation, and tries any other action (or no action after a timeout - not implemented)
    if (currentGameState == GameState::FINAL_CONFRONTATION_IMMINENT) {
        // If player input doesn't lead to using surgical item, they might be doomed.
        // This is currently handled by processInput (if they type something else or nothing valid).
        // A more robust system might have a specific "fail" condition here if no valid defense is made.
        // For now, if they don't type "use surgical_item", they'll get "unknown command" and the lunge "hangs".
        // A simple fix: if any command other than "use surgical_item" is given in this state, it's bad ending.
        // This is implicitly handled if processInput leads to no state change and the loop continues.
        // A more explicit way is needed for "no item/wrong action" in Game Story Sketch.
        // For now, `handleUseCommand` is the primary way to get ENDING_GOOD_ESCAPED.
        // If they type something else, the game currently says "Unknown command".
        // We need to ensure non-use of item leads to bad end.
        // This check can be in processInput: if in FINAL_CONFRONTATION_IMMINENT and command is NOT use surgical_item -> bad end.
        // Let's refine this in processInput or a dedicated check.
        // For Beta A, the main path is through "use surgical_item".
    }


}