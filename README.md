# The Visitor Center

## About the Game

A desperate journey to a loved one's side is cut short when your car breaks down, stranding you at the isolated Oakhaven Visitor Center. Inside, you meet the lone Guide, a man gripped by a palpable terror of the spirits he claims haunt the grounds.

To earn the parts needed to fix your car, you must agree to perform a series of "acts of respect" to appease the restless spirits. But as you delve deeper into the center's unsettling history, you'll find that the figures in the exhibits seem to watch you, and the line between superstition and a horrifying reality begins to blur.

"The Visitor Center" is a text-based psychological horror game where your choices matter. Will you placate the spirits, uncover the dark secret of Oakhaven, and escape? Or will this be the last pit stop on your journey?

## How to Play

### Prerequisites
To compile and run this game, you will need:
- A C++ compiler that supports C++17 (like `g++`)
- The `make` build tool

### Compiling and Running
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/jerrytigerxu/TheVisitorCenterTextGame.git
    ```
2.  **Navigate to the project directory:**
    ```bash
    cd TheVisitorCenterTextGame
    ```
3.  **Compile the game using the Makefile:**
    ```bash
    make
    ```
4.  **Run the executable:**
    ```bash
    ./visitor_center_game
    ```

### Basic Commands
Interact with the world using simple, two-word commands.

- **go [direction]**: Move between rooms. Directions are typically `north`, `south`, `east`, `west`, `in`, or `out`.
- **look**: Get a detailed description of your current surroundings, including any visible items or points of interest.
- **get [item]**: Pick up an item and add it to your inventory.
- **examine [item/object]**: Take a closer look at an item in your inventory or an object in the room to learn more about it.
- **use [item]**: Use an item from your inventory. This is often used to solve puzzles or interact with the environment.
- **inventory**: Check the items you are currently carrying.
- **talk to guide**: Speak with the Visitor Center's guide to get information, advance the story, or receive new tasks.
- **help**: If you're ever unsure what to do, the Guide also serves as the in-game help system. Type `help` to get a reminder of the available commands and your current objective.

