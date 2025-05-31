#ifndef GUIDE_H
#define GUIDE_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

enum class GameState; // Forward declaration

// Represents the Guide NPC
class Guide {
public:
    std::string name; 
    bool isFeigningInjury;
    // Potentially more states: ANGRY, DECEPTIVE, REVEALED, etc.

    // For dialogue and help system
    // Key: GameState or a specific dialogue trigger string
    // Value: Dialogue line or help text 
    std::map<std::string, std::string> commandExplanations;
    std::map<GameState, std::vector<std::string>> dialogueLines;

    // Constructor
    Guide(std::string name = "The Visitor Guide");

    // Interact with the Guide (talk)
    void talk(GameState currentState);

    // Provide help based on player's query or general help
    void provideHelp(const std::string& commandTopic = "general", GameState currentState = static_cast<GameState>(0));

    // Method for the Guide to change their state 
    void setFeigningInjury(bool feigning);

    // Load dialogue and help messages
    void initializeDialogue();

};

#endif // GUIDE_H