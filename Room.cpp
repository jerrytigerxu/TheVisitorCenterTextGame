#include "Room.h"
#include <algorithm>

// Constructor
Room::Room(std::string id, std::string name, std::string description) 
    : id(std::move(id)), name(std::move(name)), description(std::move(description)) {}

// Displays room information
void Room::look() const {
    // std::cout << "\n==================================================================\n"; // Moved to moveTo for better context
    std::cout << "\n--- " << name << " ---" << std::endl;
    std::cout << description << std::endl;

    bool items_present = false;
    for (const auto& item : items) {
        if (item) { // Check if unique_ptr is not null
            if(!items_present) {
                 std::cout << "\nYou see here:" << std::endl;
                 items_present = true;
            }
            std::cout << "  - " << item->id << " (" << item->name << ")" << std::endl;
        }
    }


    if (!interactive_elements.empty()) {
        std::cout << "\nAlso here:" << std::endl;
        for (const auto& element : interactive_elements) {
            std::cout << "  - " << element.name << std::endl;
        }
    }

    if (!exits.empty()) {
        std::cout << "\nExits:" << std::endl;
        for (const auto& pair : exits) {
            std::cout << "  - " << pair.first;
            // Optionally show connected room name: std::cout << " (to " << pair.second->name << ")";
            std::cout << std::endl;
        }
    } else {
        std::cout << "\nThere are no obvious exits." << std::endl;
    }
    // std::cout << "==================================================================\n"; // Moved to be before prompt in run loop
}

// Add an exit to another room
void Room::addExit(const std::string& direction, Room* room) {
    exits[direction] = room;
}

// Add an item to the room
void Room::addItem(std::unique_ptr<Item> item) {
    if (item) {
        items.push_back(std::move(item));
    }
}

// Remove an item from the room 
std::unique_ptr<Item> Room::removeItem(const std::string& itemId) {
    auto it = std::find_if(items.begin(), items.end(),
                            [&itemId](const std::unique_ptr<Item>& item_ptr) {
                                return item_ptr && item_ptr->id == itemId;
                            });
    if (it != items.end()) {
        std::unique_ptr<Item> foundItem = std::move(*it);
        items.erase(it);
        return foundItem;
    }
    return nullptr; // Item not found
}

// Add an interactive element to the room
void Room::addInteractiveElement(const InteractiveElement& element) {
    interactive_elements.push_back(element);
}

// Get a pointer to an interactive element in the room
InteractiveElement* Room::getInteractiveElement(const std::string& elementName) {
    for (auto& element : interactive_elements) {
        if (element.name == elementName) {
            return &element;
        }
    }
    return nullptr;
}

// Get a pointer to an item in the room (without removing it)
Item* Room::getItem(const std::string& itemId) {
    auto it = std::find_if(items.begin(), items.end(),
                            [&itemId](const std::unique_ptr<Item>& item_ptr) {
                                return item_ptr && item_ptr->id == itemId;
                            });
    if (it != items.end()) {
        return it->get();
    }
    return nullptr;
}