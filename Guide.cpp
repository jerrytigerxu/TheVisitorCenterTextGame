#include "Guide.h"
#include "Game.h"

// Constructor
Guide::Guide(std::string name)
    : name(std::move(name)), isFeigningInjury(false) {
        initializeDialogue();
    }

// Initialize dialogue and help messages
void Guide::initializeDialogue() {
    // Command explanations (Help System)
    commandExplanations["general"] = "You can 'go <direction/place_id>', 'look', 'examine <object/item_id>', 'get <item_id>', 'inventory', 'talk to guide', 'use <item_id>', 'help <command>', or 'quit'.";
    commandExplanations["go"] = "To move, type 'go' followed by an exit name (e.g., 'go north', 'go office', 'go enter-center'). Check 'look' for available exits.";
    commandExplanations["look"] = "Type 'look' to get a description of your current surroundings.";
    commandExplanations["examine"] = "Type 'examine' followed by the name or ID of an item or object you see (e.g., 'examine desk', 'examine gas_can').";
    commandExplanations["get"] = "Type 'get' followed by the ID of an item you see to pick it up (e.g., 'get gas_can').";
    commandExplanations["inventory"] = "Type 'inventory' to see the items you are carrying.";
    commandExplanations["talk"] = "Type 'talk to guide' to speak with me. Though, I am always listening.";
    commandExplanations["use"] = "Type 'use' followed by the ID of an item in your inventory (e.g., 'use first_aid_kit').";

    // Dialogue lines based on GameState (simplified for Beta A)
    dialogueLines[GameState::INTRO].push_back("Welcome to the Oakhaven Visitor Center. You seem... troubled. Car trouble, perhaps?");
    dialogueLines[GameState::INTRO].push_back("In a bit of a hurry, are we? You seem... burdened.");


    dialogueLines[GameState::EXPLORING_CENTER_INITIAL].push_back("Feel free to look around. Perhaps you'll find what you need... or what you deserve.");
    dialogueLines[GameState::EXPLORING_CENTER_INITIAL].push_back("Many find their way here, but not all understand the path. Clarity is essential, especially when one's mind is... elsewhere.");

    dialogueLines[GameState::PLAYER_HAS_SOME_MEANS].push_back("Making progress, I see. One step closer to your... reunion.");
    dialogueLines[GameState::PLAYER_HAS_SOME_MEANS].push_back("Your mother must be very important to you. Such strong familial bonds are... rare these days. Precious, even.");


    dialogueLines[GameState::PLAYER_HAS_ALL_MEANS].push_back("You've done well. Almost ready to rush back to your mother’s side. But before you go...");

    dialogueLines[GameState::GUIDE_FEIGNING_INJURY].push_back("(Weakly) Help me... please... First Aid... in the office...");
    
    dialogueLines[GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL].push_back("Oh, that old thing? No, my dear friend... And you, you chose to stay.");
    dialogueLines[GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL].push_back("Such profound... selflessness. Such purity. You see, the outside world is a cesspool.");
    dialogueLines[GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL].push_back("Only those who demonstrate true, selfless empathy – especially when faced with their own pressing griefs – are 'worthy.'");
    dialogueLines[GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL].push_back("The figures... they are my collection of past 'worthy' individuals, 'saved' at their moment of purest empathy.");
    dialogueLines[GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL].push_back("And such purity is too precious to be sullied by the inevitable decay of your world, or the outcome of... family illnesses.");


    dialogueLines[GameState::FINAL_CONFRONTATION_IMMINENT].push_back("You, my friend, have shown such profound compassion, even in your distress. You are truly... exemplary.");
    dialogueLines[GameState::FINAL_CONFRONTATION_IMMINENT].push_back("It is time for you to join them, to be kept perfect, forever.");


}

// Interact with the Guide
void Guide::talk(GameState currentState) {
    std::cout << "\n";
    std::cout << "\n--- " << name << " ---" << std::endl;
    if (isFeigningInjury) {
        std::cout << "(Groaning) The pain... please, hurry..." << std::endl;
        // Override normal dialogue if feigning injury
        auto it_dialogue = dialogueLines.find(GameState::GUIDE_FEIGNING_INJURY);
        if (it_dialogue != dialogueLines.end() && !it_dialogue->second.empty()) {
            std::cout << "\"" << it_dialogue->second[0] << "\"" << std::endl;
        }
        return;
    }

    auto it_dialogue = dialogueLines.find(currentState);
    if (it_dialogue != dialogueLines.end() && !it_dialogue->second.empty()) {
        std::cout << "\"" << it_dialogue->second[rand() % it_dialogue->second.size()] << "\"" << std::endl;
    } else {
        // Fallback dialogue
        switch(currentState) {
            case GameState::PLAYER_HAS_ALL_MEANS:
                std::cout << "\"" << "You have everything you need for the car. Are you ready to leave your worries behind?" << "\"" << std::endl;
                break;
            default:
                std::cout << "\"" << "Yes? Can I help you with something, or are you just... observing?" << "\"" << std::endl;
                break;
        }
    }
    std::cout << "--------------------------------" << std::endl;
}

// Provide help
void Guide::provideHelp(const std::string& commandTopic, GameState currentState) {
    std::cout << "\n";
    std::cout << "\n--- " << name << " (Help) ---" << std::endl;
    auto it = commandExplanations.find(commandTopic);
    if (it != commandExplanations.end()) {
        std::cout << it->second << std::endl;
    } else {
        std::cout << commandExplanations["general"] << std::endl;
    }

    // Add evolving help text based on GameState
    if (currentState == GameState::PLAYER_HAS_SOME_MEANS) {
        std::cout << "\"" << "Remember, what you 'get' should be what you truly need. Time is precious when loved ones wait." << "\"" << std::endl;
    } else if (currentState == GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL) {
        std::cout << "\"" << "Help? Oh, I don't think you'll needing that much longer." << "\"" << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

}

// Set Guide's state
void Guide::setFeigningInjury(bool feigning) {
    isFeigningInjury = feigning; 
}