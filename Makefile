# Makefile for The Visitor Center Game (Alpha Phase) 

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -g 


# Target executable name 
TARGET = visitor-center

# Source files 
SRCS = main.cpp


# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Default rule: build the target executable 
all: $(TARGET)

# Rule to link the object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile a .cpp source file into a .o object file 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets are targets that don't represent actual files 
.PHONY: all clean