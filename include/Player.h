#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Room.h"
#include "Item.h"

// Represents the player in the game
class Player {
public:
    // The Game class owns all Room objects, not the Player (since Player doesn't manage the Room objects, it doesn't need a smart pointer)
    Room* currentLocation;

    // The Player owns the Items in their inventory
    std::vector<std::unique_ptr<Item>> inventory;

    // Flags for specific key items or states
    bool hasGasCan;
    bool hasSpareTire;
    bool hasOilFluid;
    bool hasSurgicalDefensiveItem;
    bool hasFirstAidKit;

    // Flags for task completion
    bool hasCleanedMemorial; 
    bool hasOrganizedArchives; 
    bool hasTrimmedGarden; 

    // Constructor
    Player(Room* startLocation);

    // Moves the player to a new location
    void moveTo(Room* newLocation);

    // Adds an item to the player's inventory
    void pickUpItem(std::unique_ptr<Item> item);

    // Removes and returns an item from inventory
    std::unique_ptr<Item> dropItem(const std::string& itemId);

    // Checks if the player has a specific item by its ID
    bool hasItem(const std::string& itemId) const;

    // Gets a raw pointer to an item in inventory
    Item* getItemFromInventory(const std::string& itemId) const;

    // Displays the player's inventory
    void showInventory() const; 

    // Check if player has all "means to leave" items
    bool hasAllMeansToLeave() const;
    void updateItemFlags(const std::string& itemId, bool acquired);

};


#endif // PLAYER_H