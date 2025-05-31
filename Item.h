#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

// Represents an item that can be found, picked up, and used by the player
class Item {
public: 
    std::string name;
    std::string description;
    std::string id; 

    // Constructor 
    Item(std::string id, std::string name, std::string description);

    // --- The "virtual" keyword means that Item is a base class that could have derived (child) classes (inheritance and polymorphism)

    // Virtual destructor for potential future inheritance
    virtual ~Item() = default; 

    // Displays the item's description
    virtual void examine() const; 

    // Placeholder for using an item 
    virtual void use() const; 

};

#endif // ITEM_H