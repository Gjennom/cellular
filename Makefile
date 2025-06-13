# Compiler and flags
CXX = g++

CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Files
SRC = cell.cpp cell_functions.cpp
OBJ = $(SRC:.cpp=.o)
DEPS = cell.h
TARGET = cell

# Default target
all: $(TARGET)

# Link object files to make executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile .cpp to .o
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $iTARGETa
