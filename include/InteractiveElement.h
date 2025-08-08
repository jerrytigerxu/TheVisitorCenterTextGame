#ifndef INTERACTIVE_ELEMENT_H
#define INTERACTIVE_ELEMENT_H

#include <string>
#include <vector>
#include <iostream> 

// Represents an element in a room that the player can interact with
// Its description might change based on game events 
class InteractiveElement {
public:
    std::string name; 
    std::vector<std::string> descriptions; 
    size_t currentState;

    // Constructor 
    InteractiveElement(std::string name, const std::vector<std::string>& descs);

    // Displays the current description of the element
    void examine() const; 

    // Advances the element to its next description/state
    void advanceState();

};

#endif // INTERACTIVE_ELEMENT_H