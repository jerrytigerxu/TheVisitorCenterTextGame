#include "Item.h"

// Constructor 
Item::Item(std::string id, std::string name, std::string description)
    : id(std::move(id)), name(std::move(name)), description(std::move(description)) {}

// Displays the item's description
void Item::examine() const {
    std::cout << description << std::endl;
}


// Placeholder for using an item
void Item::use() const {
    std::cout << "You try to use the " << name << ", but nothing specific happens." << std::endl;
}