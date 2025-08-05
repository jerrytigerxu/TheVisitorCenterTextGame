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

    // Initial dialogue when player first meets the Guide.
    dialogueLines[GameState::INTRO].push_back("Oh! A visitor... I... I'm sorry for the state of things. The air has been so heavy lately. Since you've arrived... the spirits... they feel your presence, and they're not pleased.");

    // Dialogue for Task 1
    dialogueLines[GameState::AWAITING_TASK_1].push_back("This place remembers. The Pioneer Family exhibit honors those who first settled Oakhaven. To show the spirits you mean no harm, perhaps a simple act of care is needed. Wiping away the dust from their memorial would speak volumes. Do this, and I feel they will quiet enough for me to get you the gas can from the storage room.");

    // Dialogue for Task 2
    dialogueLines[GameState::AWAITING_TASK_2].push_back("That noise... the music box... they're still not satisfied. The archives... a place of history and order, has fallen into disarray. They hate chaos. If you could organize the scattered papers, put things back in order... it would soothe them. The spare tire is in the West Wing; this act of respect should grant us passage.");

    // Dialogue for Task 3
    dialogueLines[GameState::AWAITING_TASK_3].push_back("Did you see them move? It's getting worse... The music box that fell... it belonged to a little girl, one of the first to be lost here. Her spirit is the most restless. If you could repair it... maybe its song could bring some peace to her, and to this place. The oil fluid is in my office. A final act of respect like this might be all we need.");

    // Dialogue for Task 4
    dialogueLines[GameState::AWAITING_TASK_4].push_back("The tableau... they've arranged themselves... it's a warning. My plan failed. I... I'm so sorry. There is one last thing we can try. A desperate, final act. A candlelight vigil. To show our sorrow, our respect for their final moments. In the office... please. It's our only chance.");

    // Endgame dialogue remains
    dialogueLines[GameState::GUIDE_FACES_VENGEANCE].push_back("(Screaming) You've desecrated the vigil! Now they will have their vengeance on ME!");
    dialogueLines[GameState::PLAYER_RETURNS_GUIDE_UNHARMED_REVEAL].push_back("Oh, that old thing? No, my dear friend... that wasn't for me. And you... you chose to stay.");
    dialogueLines[GameState::FIGURES_REVEALED].push_back("The figures... they are my collection of past 'worthy' individuals, 'saved' at their moment of purest empathy.");
    dialogueLines[GameState::FINAL_CONFRONTATION_IMMINENT].push_back("You, my friend, have shown such profound compassion. It is time for you to join them, to be kept perfect, forever.");


}

// Interact with the Guide
std::string Guide::getDialogue(GameState currentState) const {
    auto it_dialogue = dialogueLines.find(currentState);
    if (it_dialogue != dialogueLines.end() && !it_dialogue->second.empty()) {
        return it_dialogue->second[rand() % it_dialogue->second.size()];
    } else {
        return "He just stares at you, a look of profound sorrow on his face.";

    }
}

// Provide help
void Guide::provideHelp(const std::string& commandTopic, [[maybe_unused]] GameState currentState) {
    std::cout << "\n";
    std::cout << "\n--- " << name << " (Help) ---" << std::endl;
    auto it = commandExplanations.find(commandTopic);
    if (it != commandExplanations.end()) {
        std::cout << it->second << std::endl;
    } else {
        std::cout << commandExplanations["general"] << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

}

// Set Guide's state
void Guide::setFeigningInjury(bool feigning) {
    isFeigningInjury = feigning; 
}