# Compiler
CXX = g++
# Compiler flags
# -std=c++17: Use C++17 standard
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -g: Add debugging information
CXXFLAGS = -std=c++17 -Wall -Wextra -g
# Linker flags (none needed for this simple case yet)
LDFLAGS =

# Source files
SRCS = main.cpp Game.cpp Room.cpp Player.cpp Item.cpp Guide.cpp InteractiveElement.cpp
# Object files (derived from SRCS)
OBJS = $(SRCS:.cpp=.o)
# Executable name
TARGET = visitor_center_game

# Default target: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile a .cpp file into a .o file
# $< is the first prerequisite (the .cpp file)
# $@ is the target (the .o file)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (targets that are not actual files)
.PHONY: all clean