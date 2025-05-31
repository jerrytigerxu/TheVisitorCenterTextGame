#define INTERACTIVE_ELEMENT_H

#include <string>
#include <iostream> 

// Represents an element in a room that the player can interact with
// Its description might change based on game events 
class InteractiveElement {
public:
    std::string name; 
    std::string initial_description;
    std::string revealed_description;
    bool is_revealed; 

    // Constructor 
    InteractiveElement(std::string name, std::string initial_desc, std::string revealed_desc = "");

    // Displays the current description of the element
    void examine() const; 

    // Method to change the element's state to 'revealed'
    void reveal();

};

#endif // INTERACTIVE_ELEMENT_H