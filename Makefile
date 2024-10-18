# Compiler
CXX := clang++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
DBGFLAGS := -g $(CXXFLAGS)

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
INSTALL_DIR := /usr/local/bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable name
TARGET := $(BUILD_DIR)/terminder
DEBUG_TARGET := $(BUILD_DIR)/terminder_debug

# Default target
all: $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Debug build
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SRCS)
	@mkdir -p $(@D)
	$(CXX) $(DBGFLAGS) -I$(INCLUDE_DIR) $^ -o $@

# Run the program
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Debug with with LLDB
debug-lldb: $(DEBUG_TARGET)
	lldb $(DEBUG_TARGET)

# Installation
install: $(TARGET)
	install -d $(INSTALL_DIR)
	install -m 755 $(TARGET) $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)$(notdir $(TARGET))

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all run clean debug debug-lldb install uninstall
