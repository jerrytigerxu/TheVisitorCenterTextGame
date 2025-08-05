# Makefile for The Visitor Center Text Game

# -----------------
# Configuration
# -----------------

# Compiler
CXX = g++

# Compiler flags: -std=c++17 for modern C++, -Wall for all warnings, -g for debugging symbols
CXXFLAGS = -std=c++17 -Wall -g

# Project directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Name of the final executable
TARGET = visitor_center_game

# -----------------
# File Discovery
# -----------------

# Automatically find all .cpp files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Create a list of corresponding object files (.o) to be placed in the object directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Add the include directory to the compiler's search path for headers
CXXFLAGS += -I$(INCLUDE_DIR)

# -----------------
# Build Rules
# -----------------

# The default target, 'all', builds the final executable.
all: $(TARGET)

# Rule to link the final executable from all the object files.
$(TARGET): $(OBJS)
	@echo "Linking executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete. Run with ./$(TARGET)"

# This is a master dependency rule. 
$(OBJS): | $(OBJ_DIR)

# This is the pattern rule for compilation. 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# This rule tells make HOW to create the object directory. 
$(OBJ_DIR):
	@echo "Creating object directory..."
	mkdir -p $(OBJ_DIR)

# -----------------
# Utility Rules
# -----------------

# Rule to clean up all compiled files.
clean:
	@echo "Cleaning project..."
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo "Clean complete."

# Phony targets are not actual files. They are just names for commands.
.PHONY: all clean