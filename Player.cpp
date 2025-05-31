#include "Player.h"
#include <algorithm>

// Constructor
Player::Player(Room* startLocation) 
    : currentLocation(startLocation),
    hasGasCan(false),
    hasSpareTire(false),
    hasOilFluid(false),
    hasSurgicalDefensiveItem(false),
    hasFirstAidKit(false) {}

// Moves the player to a new location 
void Player::moveTo(Room* newLocation) {
    currentLocation = newLocation;
    if (currentLocation) {
        currentLocation->look();
    }
}

// Adds an item to the player's inventory
void Player::pickUpItem(std::unique_ptr<Item> item) {
    if (item) {
        std::cout << "You picked up the " << item->name << "." << std::endl;
        updateItemFlags(item->id, true);
        inventory.push_back(std::move(item));
    }
}

// Removes and returns an item from inventory
std::unique_ptr<Item> Player::dropItem(const std::string& itemId) {
    auto it = std::find_if(inventory.begin(), inventory.end(),
                           [&itemId](const std::unique_ptr<Item>& item_ptr) {
                               return item_ptr && item_ptr->id == itemId;
                           });

    if (it != inventory.end()) {
        std::unique_ptr<Item> foundItem = std::move(*it);
        inventory.erase(it);
        std::cout << "You dropped the " << foundItem->name << "." << std::endl;
        updateItemFlags(foundItem->id, false);
        return foundItem;
    }
    std::cout << "You don't have a '" << itemId << "' to drop." << std::endl;
    return nullptr;
}

// Checks if the player has a specific item by its ID
bool Player::hasItem(const std::string& itemId) const {
    for (const auto& item_ptr : inventory) {
        if (item_ptr && item_ptr->id == itemId) {
            return true;
        }
    }
    return false;
}

// Gets a raw pointer to an item in inventory
Item* Player::getItemFromInventory(const std::string& itemId) const {
    for (const auto& item_ptr : inventory) {
        if (item_ptr && item_ptr->id == itemId) {
            return item_ptr.get();
        }
    }
    return nullptr;
}

// Displays the player's inventory
void Player::showInventory() const {
    if (inventory.empty()) {
        std::cout << "Your inventory is empty." << std::endl;
    } else {
        std::cout << "\n--- Inventory ---" << std::endl;
        for (const auto& item : inventory) {
            if (item) {
                std::cout << "  - " << item->name << " (" << item->id << ")" << std::endl;
            }
        }
        std::cout << "------------------------" << std::endl;
    }
}

// Updates specific item flags based on item ID
void Player::updateItemFlags(const std::string& itemId, bool acquired) {
    if (itemId == "gas_can") hasGasCan = acquired;
    else if (itemId == "spare_tire") hasSpareTire = acquired;
    else if (itemId == "oil_fluid") hasOilFluid = acquired;
    else if (itemId == "surgical_item") hasSurgicalDefensiveItem = acquired;
    else if (itemId == "first_aid_kit") hasFirstAidKit = acquired;
}

// Check if player has all "means to leave" items
bool Player::hasAllMeansToLeave() const {
    return hasGasCan && hasSpareTire && hasOilFluid;
}