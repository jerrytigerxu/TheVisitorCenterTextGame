#include "InteractiveElement.h"

// Constructor
InteractiveElement::InteractiveElement(std::string name, std::string initial_desc, std::string revealed_desc)
    : name(std::move(name)),
        initial_description(std::move(initial_desc)),
        revealed_description(std::move(revealed_desc)),
        is_revealed(false) {}

// Displays the current description of the element
void InteractiveElement::examine() const {
    if (is_revealed && !revealed_description.empty()) {
        std::cout << revealed_description << std::endl;
    } else {
        std::cout << initial_description << std::endl;
    }
}

// Method to change the element's state to be revealed
void InteractiveElement::reveal() {
    is_revealed = true;
    // Potentially add a message here if revealing it has an immediate effect/text
}