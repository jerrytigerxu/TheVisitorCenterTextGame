#include "InteractiveElement.h"

// Constructor
InteractiveElement::InteractiveElement(std::string name, const std::vector<std::string>& descs)
    : name(std::move(name)),
    descriptions(descs),
    currentState(0) {}

// Displays the current description of the element
void InteractiveElement::examine() const {
    if (!descriptions.empty() && currentState < descriptions.size()) {
        std::cout << descriptions[currentState] << std::endl;
    } else {
        std::cout << "You look at the " << name << ", but nothing seems out of the ordinary." << std::endl;
    }
}

// Method to advance the element to its next state
void InteractiveElement::advanceState() {
    // Check if there is a next state to advance to 
    if (currentState + 1 < descriptions.size()) {
        currentState++;
    }
    
}