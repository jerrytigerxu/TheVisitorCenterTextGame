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
    allRooms.push_back(std::make_unique<Room>("main_hall", "Main Hall", "A large, dusty main hall stretches before you. Lifelike figures stand in silent watch from shadowy alcoves. An older man, the Guide, is here. He eyes you curiously. A small, ornate music box sits on a high shelf."));
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
        // The Figures: Now with multiple descriptions for the scares.
        hall->addInteractiveElement(InteractiveElement("figures", {
            /* State 0: Initial */ "The 'exhibits' are figures depicting scenes from Oakhaven's history. From a distance, they look like wax, but up close, the detail is unnerving. The texture of the skin is too porous, the hair seems too fine, and the eyes have a glassy, wet-looking sheen that makes you want to look away.",
            /* State 1: Scare 1 */ "You look at the figures again. Your blood runs cold. You could swear one of the heads is tilted slightly, its glassy eyes now aimed directly at the entrance to the storage room. It must be a trick of the light.",
            /* State 2: Scare 2 */ "It's not your imagination. The figures have definitely moved. They are now clustered together, forming a menacing tableau aimed at the center of the room. Their silent judgment is suffocating.",
            /* State 3: Revealed */ "The 'figures' are no exhibits. They are horrifyingly preserved human bodies, skin like leather, eyes fixed in a moment of past terror. The Guide's 'collection'."
        }));
        
        // The Guide
        hall->addInteractiveElement(InteractiveElement("guide", {
            "The Visitor Guide is an older man, with eyes that dart nervously around the room. He carries the weight of this place on his shoulders, an air of profound fear about him.",
            "The Guide's fear is gone, replaced by a triumphant, predatory smile. He is the master of this macabre gallery, the hunter who has successfully lured his prey."
        }));

        // The Music Box (Task 1 Scare)
        hall->addInteractiveElement(InteractiveElement("music_box", {
            "A small, ornate music box sits on a high shelf, covered in a thin layer of dust.",
            "Shards of wood and metal litter the floor where the music box used to be. It's completely destroyed."
        }));

        // Task 1 Object
        hall->addInteractiveElement(InteractiveElement("memorial", {
            "A dusty memorial plaque dedicated to the 'Pioneers of Oakhaven'. It's hard to read the names under the grime.",
            "The memorial plaque is now clean, the names of the lost gleaming faintly in the dim light."
        }));
    }

    Room* storage = findRoomById("storage_room");
    if (storage) {
        storage->addInteractiveElement(InteractiveElement("archives", {
            "A collection of dusty photo albums and records, scattered chaotically across a table.",
            "The archives are now neatly stacked. A lingering sense of order has been restored."
        }));
    }

    Room* west_wing = findRoomById("west_wing");
    if (west_wing) {
        west_wing->addInteractiveElement(InteractiveElement("memorial_garden", {
            "Thorny, overgrown vines choke the memorial stones in the garden area, obscuring them from view.",
            "The thorny vines have been trimmed back, revealing the names on the stones beneath."
        }));
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

void Game::typeOut(const std::string& text, bool isDialogue) {
    if (isDialogue) std::cout << "\"";
    for (const char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(35));
    }
    if (isDialogue) std::cout << "\"";
    std::cout << std::endl;
}

void Game::enterCutscene() {
    isInCutscene = true;
    std::cout << "\n";
}

void Game::exitCutscene() {
    isInCutscene = false;
}

void Game::displayIntro() {
    enterCutscene();
    std::cout << "----------------------------------------------------------" << std::endl;
    typeOut("           THE VISITOR CENTER");
    std::cout << "----------------------------------------------------------" << std::endl;
    
    typeOut("Your heart pounds with anxiety. Racing to your gravely ill mother, your chosen shortcut has led to disaster.");
    typeOut("Your car sputters and dies near the Oakhaven Visitor Center – an isolated, dilapidated structure exuding an unnerving stillness.");
    typeOut("Miles from anywhere, with a failing phone signal, the Center is your only hope.");
    exitCutscene();
    
    // Player's location look() is now called from moveTo, which is called from setupGame
    if (player.currentLocation) {
        player.currentLocation->look();
    }
    transitionToState(GameState::INTRO);
}

void Game::transitionToState(GameState newState) {
    currentGameState = newState;

    // Specific actions on entering a new state
    switch (newState) {
        case GameState::FIRST_ENCOUNTER_WITH_GUIDE:
            enterCutscene();
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            typeOut("\n(A thought crosses your mind: This man is clearly unwell... but he's my only way out of here. I'll play along.)");
            typeOut("\nYou should probably talk to him again to see what he wants you to do.");
            exitCutscene();
            transitionToState(GameState::AWAITING_TASK_1);
            break;
        
        case GameState::GUIDE_FACES_VENGEANCE:
            enterCutscene();
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            exitCutscene();
            transitionToState(GameState::CHOICE_POINT_LEAVE_OR_HELP);
            break;
        case GameState::TASK_1_COMPLETE:
            {
                enterCutscene();
                typeOut("As you finish, a sudden, loud CRASH from across the hall makes you jump.");
                typeOut("The small music box has fallen from its shelf, shattering on the floorboards.");
                typeOut("Your heart hammers against your ribs. It must have been precariously balanced. It had to be.");
                
                InteractiveElement* musicBox = player.currentLocation->getInteractiveElement("music_box");
                if (musicBox) musicBox->advanceState();

                typeOut("\n--- " + guide.name + " ---", false);
                typeOut("He flinches at the sound, his face pale. 'A good sign,' he whispers, though he sounds anything but convinced. 'The spirits... they noticed. I can now unlock the storage room for you. The gas can should be in there.'");
                exitCutscene();
                transitionToState(GameState::AWAITING_TASK_2);
            }
            break;
        case GameState::CHOICE_POINT_LEAVE_OR_HELP:
            enterCutscene();
            typeOut("\nWith the sounds of the attack echoing behind you, your mind races. Your mother... waiting.");
            typeOut("But you caused this. You doomed him. What kind of person would you be if you just left?");
            typeOut("Your choices: 'leave' or 'help'.");
            exitCutscene();
            break;

        case GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL:
            enterCutscene();
            guide.setFeigningInjury(false); 
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            exitCutscene();
            transitionToState(GameState::FIGURES_REVEALED);
            break;

        case GameState::FIGURES_REVEALED:
            enterCutscene();
            if (player.currentLocation) {
                InteractiveElement* figures = player.currentLocation->getInteractiveElement("figures");
                if (figures) figures->advanceState();
            }
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            exitCutscene();
            transitionToState(GameState::FINAL_CONFRONTATION_IMMINENT);
            break;

        case GameState::FINAL_CONFRONTATION_IMMINENT:
            enterCutscene();
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            typeOut("\nHe lunges towards you!");
            exitCutscene();
            break;

        case GameState::ENDING_NOT_WORTHY:
        case GameState::ENDING_GOOD_ESCAPED:
        case GameState::ENDING_BAD_VICTIM:
            displayEnding(currentGameState);
            break;
        default:
            break;
    }
}

void Game::displayEnding(GameState endingType) {
    enterCutscene();
    switch (endingType) {
        case GameState::ENDING_NOT_WORTHY:
            typeOut("\n--- ENDING 1: The Unworthy ---");
            typeOut("The image of your mother, pale and still in a hospital bed, burned in your mind. Guilt was a hot stone in your gut, but the primal need to reach your family was a tidal wave.");
            typeOut("As you flee the Visitor Center, the Guide's voice, no longer weak, echoes from within:");
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            typeOut("You escape, the words a brand, but the drive to the hospital consuming all else.");
            break;
        case GameState::ENDING_GOOD_ESCAPED:
            typeOut("\n--- ENDING 2: The Escape ---");
            typeOut("With a desperate surge of adrenaline, you use the surgical instrument!");
            typeOut("The Guide recoils, momentarily stunned, giving you the chance you need.");
            typeOut("You scramble past him and out of the horrific visitor center, not daring to look back.");
            typeOut("Forever scarred, you carry the weight of Oakhaven, but also a desperate hope as you race towards your mother.");
            break;
        case GameState::ENDING_BAD_VICTIM:
            typeOut("\n--- ENDING 3: The Collection ---");
            typeOut("You try to react, but the Guide is too quick, his earlier frailty a complete deception.");
            typeOut("His smile is the last thing you see.");
            typeOut("--- " + guide.name + " ---", false);
            typeOut(guide.getDialogue(currentGameState), true);
            typeOut("The Oakhaven Visitor Center had claimed another exhibit. Far away, a hospital vigil continued, unaware.");
            break;
        default:
            typeOut("Error: Unknown ending type.");
            break;
    }
    exitCutscene();
    gameOver = true;
    currentGameState = GameState::GAME_OVER;
}

// Main game loop
void Game::run() {
    displayIntro();
    std::string inputLine;

    while (!gameOver) {
        if (currentGameState == GameState::GAME_OVER) break;

        if (!isInCutscene) {
            std::cout << "\n";
            if (player.currentLocation) {
                std::cout << "[" << player.currentLocation->name <<"] > ";
            } else {
                std::cout << "[Unknown location] > ";
            }
        }
        
        if (!std::getline(std::cin, inputLine)) {
            if (std::cin.eof()) break;
            std::cerr << "Input error. Quitting." << std::endl;
            break;
        }

        if (isInCutscene || inputLine.empty()) continue;

        processInput(inputLine);
        updateGame();
    }

    std::cout << "\n--- Thank you for playing The Visitor Center! ---" << std::endl;
}

std::vector<std::string> Game::parseCommand(const std::string& rawInput) {
    std::vector<std::string> words;
    std::string input = rawInput;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    std::stringstream ss(input);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

void Game::processInput(const std::string& rawInput) {
    if (gameOver) return;

    std::vector<std::string> words = parseCommand(rawInput);
    if (words.empty()) return;

    std::string command = words[0];
    std::cout << "\n==================================================================\n";

    if (currentGameState == GameState::FINAL_CONFRONTATION_IMMINENT) {
        if (command == "use" && words.size() > 1 && words[1] == "surgical_item") {
            if (player.hasItem("surgical_item")) {
                transitionToState(GameState::ENDING_GOOD_ESCAPED);
            } else {
                enterCutscene();
                typeOut("You don't have that!");
                exitCutscene();
                transitionToState(GameState::ENDING_BAD_VICTIM);
            }
        } else {
            enterCutscene();
            typeOut("You panic. Your action is ineffective as the Guide overwhelms you.");
            exitCutscene();
            transitionToState(GameState::ENDING_BAD_VICTIM);
        }
        return;
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
    } else if (command == "clean") {
        handleCleanCommand(words);
    } else if (command == "organize") {
        handleOrganizeCommand(words);
    } else if (command == "trim") {
        handleTrimCommand(words);
    } else if (command == "leave" || command == "help") { // Simplified choice commands
        if (currentGameState == GameState::CHOICE_POINT_LEAVE_OR_HELP) {
             handleChooseCommand({command}); // Pass the command directly
        } else {
            std::cout << "You can't do that right now." << std::endl;
        }
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
        player.pickUpItem(std::make_unique<Item>("gas_can", "Gas Can (Debug)", "Debug Gas"));
        player.pickUpItem(std::make_unique<Item>("spare_tire", "Spare Tire (Debug)", "Debug Tire"));
        player.pickUpItem(std::make_unique<Item>("oil_fluid", "Oil Fluid (Debug)", "Debug Oil"));
        if (!player.hasItem("surgical_item")) {
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

    // Room Unlocking Logic
    if (destination_key == "storage" && currentGameState < GameState::AWAITING_TASK_2) {
        std::cout << "The Guide has not unlocked that door for you yet. It's locked." << std::endl;
        return; 
    }
    if (destination_key == "west" && currentGameState < GameState::AWAITING_TASK_3) {
        std::cout << "That part of the center is sealed off. The Guide hasn't opened it." << std::endl;
        return;
    }
    if (destination_key == "office" && currentGameState < GameState::AWAITING_TASK_4) {
        std::cout << "The Guide's office is securely locked." << std::endl;
        return;
    }

    if (player.currentLocation && player.currentLocation->exits.count(destination_key)) {
        Room* nextRoom = player.currentLocation->exits[destination_key];
        player.moveTo(nextRoom);

        if (nextRoom && nextRoom->id == "main_hall" && currentGameState == GameState::INTRO) {
            transitionToState(GameState::FIRST_ENCOUNTER_WITH_GUIDE);
        } else if (currentGameState == GameState::TASK_2_COMPLETE) {
                InteractiveElement* figures = nextRoom->getInteractiveElement("figures");
                if (figures && figures->currentState == 0) {
                    figures->advanceState();
                    enterCutscene();
                    typeOut("You re-enter the main hall. A chill crawls up your spine. Something feels... wrong.");
                    typeOut("(My heart is pounding. Did... did they just move? No. It's just my mind playing tricks on me. It has to be.)");
                    exitCutscene();
                }
            }
            else if (currentGameState == GameState::MENACING_TABLEAU) {
                 InteractiveElement* figures = nextRoom->getInteractiveElement("figures");
                if (figures && figures->currentState == 1) {
                    figures->advanceState(); // Advance to Scare 2 description
                    enterCutscene();
                    typeOut("You step back into the hall and the sight before you steals the air from your lungs.");
                    typeOut("It's not your imagination. The figures have moved. They are now clustered together in the center of the room, a silent, menacing jury. Their glassy eyes are all fixed on you.");
                    typeOut("The Guide looks at them, his face a mask of pure terror.");
                    exitCutscene();
                }
            }
        else if (nextRoom && nextRoom->id == "main_hall" && currentGameState == GameState::PLAYER_FOUND_MEDKIT) {
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
    if (targetName == "the" && words.size() > 2) {
        targetName = words[2];
    }

    if (player.currentLocation) {
        Item* roomItem = player.currentLocation->getItem(targetName);
        if (roomItem) { roomItem->examine(); return; }
        
        InteractiveElement* element = player.currentLocation->getInteractiveElement(targetName);
        if (element) {
            element->examine();
            return;
        }
    }
    
    Item* invItem = player.getItemFromInventory(targetName);
    if (invItem) { invItem->examine(); return; }

    if (targetName == "guide") {
        if (player.currentLocation->getInteractiveElement("guide")) {
            player.currentLocation->getInteractiveElement("guide")->examine();
        } else {
            std::cout << "The Guide isn't here." << std::endl;
        }
        return;
    }

    std::cout << "You don't see any '" << targetName << "' here to examine, nor are you carrying it." << std::endl;
}

void Game::handleGetCommand(const std::vector<std::string>& words) {
    if (words.size() < 2) { std::cout << "Get what?" << std::endl; return; }
    std::string itemId = words[1];

    if (player.currentLocation && player.currentLocation->getItem(itemId)) {
        std::unique_ptr<Item> item = player.currentLocation->removeItem(itemId);
        
        if (item->id == "oil_fluid" && !surgicalItemSpawned) {
             Room* officeRoom = findRoomById("office");
             if(officeRoom) {
                officeRoom->addItem(std::make_unique<Item>("surgical_item", "Surgical Instrument", "An antique surgical instrument, surprisingly well-maintained. It was tucked away near where the oil was. Almost... waiting."));
                surgicalItemSpawned = true;
                enterCutscene();
                typeOut("As you pick up the oil, a glint of metal from a shadowy corner catches your eye.");
                exitCutscene();
             }
        }
        
        if(item->id == "first_aid_kit" && currentGameState == GameState::CHOICE_POINT_LEAVE_OR_HELP) {
            transitionToState(GameState::PLAYER_FOUND_MEDKIT);
            enterCutscene();
            typeOut("You have the First Aid Kit. You should return to the Guide in the main hall.");
            exitCutscene();
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
    if ((words.size() > 2 && (words[1] == "to" || words[1] == "with") && words[2] == "guide") ||
        (words.size() > 1 && words[1] == "guide")) {
        if (player.currentLocation && player.currentLocation->id == "main_hall") {
            if (currentGameState == GameState::TASK_1_COMPLETE) {
                enterCutscene();
                typeOut("--- " + guide.name + " ---", false);
                typeOut("Thank you. I... I can feel their anger lessening slightly. As promised, I have unlocked the storage room. The gas can should be in there. But... they are still not satisfied. The archives in that room... a place of history and order, has fallen into disarray. They hate chaos. If you could organize the scattered papers, it would soothe them further. This act of respect should grant us passage to the West Wing, where the spare tire is kept.", true);
                exitCutscene();
                transitionToState(GameState::AWAITING_TASK_2);
                return;
            }
            if (currentGameState == GameState::TASK_2_COMPLETE) {
                enterCutscene();
                typeOut("--- " + guide.name + " ---", false);
                typeOut("You saw it too, didn't you? The figures moving... It's getting worse. The spirits are more agitated than ever. There is one final act. A memorial garden in the West Wing has become overgrown. Tending to it might be the show of respect we need to finally calm them. The oil fluid you need is in my office. Please, this might be our last chance.", true);
                exitCutscene();
                transitionToState(GameState::AWAITING_TASK_3);
                return;
            }
            // "False Hope" dialogue after Task 3
            if (currentGameState == GameState::TASK_3_COMPLETE_FALSE_HOPE) {
                enterCutscene();
                typeOut("--- " + guide.name + " ---", false);
                typeOut("It's quiet... too quiet. I think... I think we've done it. The air feels lighter. Thank you. Truly. As promised, my office is unlocked. The oil fluid is in there. Get it, and you can finally leave this dreadful place.", true);
                typeOut("(A wave of relief washes over you. It's finally over. You just need to get the last part and you can go home.)");
                exitCutscene();
                transitionToState(GameState::MENACING_TABLEAU); // Set up the next trigger
                return;
            }
            // "Desperate Last Resort" dialogue after the tableau
            if (currentGameState == GameState::MENACING_TABLEAU) {
                enterCutscene();
                typeOut("--- " + guide.name + " ---", false);
                typeOut("It wasn't enough! They're angrier than ever! My plan... it failed! I... I'm so sorry. There is one last thing we can try. A desperate, final act. A candlelight vigil. To show our sorrow for their final moments. In the office... please. It's our only chance.", true);
                exitCutscene();
                transitionToState(GameState::AWAITING_TASK_4);
                return;
            }
            
            // Default dialogue
            enterCutscene();
            typeOut("--- " + guide.name + " ---", false);
            std::string dialogue = guide.getDialogue(currentGameState);
            typeOut(dialogue, true);
            
            // Special player thought for intro
            if (currentGameState == GameState::AWAITING_TASK_1) {
                typeOut("(A thought crosses your mind: This man is clearly unwell... but he's my only hope. I'll play along.)");
            }

            exitCutscene();

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
    
    // --- DEMO of how cutscenes will work for tasks ---
    if (itemId == "gas_can" && currentGameState == GameState::AWAITING_TASK_1) {
        transitionToState(GameState::TASK_1_COMPLETE);
        enterCutscene();
        typeOut("This isn't how it works. The Guide asked you to perform an act of respect, not just use an item.");
        typeOut("Perhaps you should 'examine' the 'figures' to know what to do.");
        exitCutscene();
        // Set state back for demonstration purposes
        transitionToState(GameState::AWAITING_TASK_1);
        return;
    }

    std::cout << "You try to use the " << itemId << ", but nothing specific happens." << std::endl;
}

void Game::handleChooseCommand(const std::vector<std::string>& words) {
    if (currentGameState != GameState::CHOICE_POINT_LEAVE_OR_HELP) {
        std::cout << "There's no specific choice to make right now with that command." << std::endl;
        return;
    }
    if (words.empty()) {
        std::cout << "Choose what? ('leave' or 'help')" << std::endl;
        return;
    }
    std::string choice = words[0];
    if (choice == "leave") {
        transitionToState(GameState::ENDING_NOT_WORTHY);
    } else if (choice == "help") {
        enterCutscene();
        typeOut("You decide to help the Guide. He weakly gestures towards the office, muttering about a First Aid Kit.");
        exitCutscene();
        transitionToState(GameState::PLAYER_CHOOSES_HELP_SEARCH_MEDKIT);
    } else {
        std::cout << "That's not a valid choice here. Try 'leave' or 'help'." << std::endl;
    }
}

void Game::handleCleanCommand(const std::vector<std::string>& words) {
    if (words.size() < 2 || words[1] != "memorial") {
        std::cout << "Clean what? (Perhaps you should 'clean memorial'?)" << std::endl;
        return;
    }
    if (player.currentLocation->id != "main_hall") {
        std::cout << "There is no memorial to clean here." << std::endl;
        return; 
    }
    if (currentGameState == GameState::AWAITING_TASK_1) {
        if (!player.hasCleanedMemorial) {
            player.hasCleanedMemorial = true;

            // Update the memorial's description to be clean
            InteractiveElement* memorial = player.currentLocation->getInteractiveElement("memorial");
            if (memorial) {
                memorial->advanceState();
            }

            std::cout << "You carefully wipe the dust and grime from the memorial plaque. It's a small gesture, but it feels significant." << std::endl;
            transitionToState(GameState::TASK_1_COMPLETE);
        } else {
            std::cout << "You've already cleaned the memorial." << std::endl;
        }
    } else {
        std::cout << "That doesn't seem necessary right now." << std::endl; 
    }
}

void Game::handleOrganizeCommand(const std::vector<std::string>& words) {
    if (words.size() < 2 || words[1] != "archives") {
        std::cout << "Organize what? (Perhaps 'organize archives'?)" << std::endl;
        return; 
    }
    if (player.currentLocation->id != "storage_room") {
        std::cout << "There are no archives to organize here." << std::endl; 
        return;
    }
    if (currentGameState == GameState::AWAITING_TASK_2) {
        if (!player.hasOrganizedArchives) {
            player.hasOrganizedArchives = true;
            InteractiveElement* archives = player.currentLocation->getInteractiveElement("archives");
            if (archives) archives->advanceState();

            enterCutscene();
            typeOut("You spend a few minutes stacking the old photo albums and papers into neat piles. The room feels a little less chaotic now.");
            typeOut("You should return to the Guide in the main hall.");
            exitCutscene();
            transitionToState(GameState::TASK_2_COMPLETE);
        } else {
            std::cout << "You've already organized the archives." << std::endl;
        }
    } else {
        std::cout << "That doesn't seem necessary right now." << std::endl;
    }
}

void Game::handleTrimCommand(const std::vector<std::string>& words) {
    if (words.size() < 2 || words[1] != "overgrowth") {
        std::cout << "Trim what? (Perhaps 'trim overgrowth'?)" << std::endl;
        return;
    }
    if (player.currentLocation->id != "west_wing") {
        std::cout << "There is no overgrowth to trim here." << std::endl;
        return; 
    }
    if (currentGameState == GameState::AWAITING_TASK_3) {
        if (!player.hasTrimmedGarden) {
            player.hasTrimmedGarden = true;
            InteractiveElement* overgrowth = player.currentLocation->getInteractiveElement("overgrowth");
            if (overgrowth) overgrowth->advanceState();
            
            enterCutscene();
            typeOut("You carefully trim back the thorny vines, revealing the names on the memorial stones. A profound sadness seems to lift from the area.");
            typeOut("You should return to the Guide in the main hall.");
            exitCutscene();
            transitionToState(GameState::TASK_3_COMPLETE_FALSE_HOPE);
        } else {
            std::cout << "You've already trimmed the overgrowth." << std::endl;
        }
    } else {
        std::cout << "That doesn't seem necessary right now." << std::endl; 
    }
}

// Updates game state
void Game::updateGame() {
    if (gameOver) return;

    // This function will be used later to trigger timed events or passive state changes.
}