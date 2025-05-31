#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include "Item.h"
#include "InteractiveElement.h"

// Represents a location in the game
// Rooms are owned by the Game class (via std::unique_ptr)
// Exits are raw pointers as they don't imply ownership
class Room {
public:
    std::string id;
    std::string name;
    std::string description;

    // Exits to other rooms: key is direction, value is raw pointer to the Room
    std::map<std::string, Room*> exits; 

    // Items currently in this room. The Room owns these items via unique_ptr
    std::vector<std::unique_ptr<Item>> items;

    // Interactive elements in this room
    std::vector<InteractiveElement> interactive_elements;

    // Constructor
    Room(std::string id, std::string name, std::string description);

    // Displays room information (name, description, items, interactive elements, exits)
    void look() const; 

    // Add an exit to another room
    void addItem(std::unique_ptr<Item> item);

    // Remove an item from the room (e.g., when player picks it up)
    // Returns the item or nullptr if not found
    std::unique_ptr<Item> removeItem(const std::string& itemId);

    // Add an interactive element to the room
    void addInteractiveElement(const InteractiveElement& element);

    // Get a pointer to an interactive element in the room 
    InteractiveElement* getInteractiveElement(const std::string& elementName);

    // Get a pointer to an item in the room (without removing it)
    Item* getItem(const std::string& itemId);

};



#endif // ROOM_H