# Compiler settings
CXX := clang++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Wpedantic

# Directories
SRCDIR := src
BUILDDIR := build

# Source files
SRCS := $(wildcard $(SRCDIR)/*.cpp)

# Object files
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Binary name
TARGET := $(BUILDDIR)/task

# Default target
all: $(TARGET)

# Compile object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean the build directory
clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
