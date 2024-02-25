# Makefile

# Compiler and compiler flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Directories
SRCDIR = src
BUILDDIR = build

# Target executable name
TARGET = $(BUILDDIR)/task

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Header files
HEADERS = $(wildcard $(SRCDIR)/*.h)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compiling
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Creating build directory
$(shell mkdir -p $(BUILDDIR))

# Clean up
clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
