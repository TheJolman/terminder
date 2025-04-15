# Compiler
CXX ?= clang++

# Compiler flags
CXXFLAGS ?= -std=c++20 -Wall -Wextra -pedantic
DBGFLAGS := -g $(CXXFLAGS)

# Output dirs
PREFIX ?= /usr/local
DESTDIR ?=
BINDIR = $(DESTDIR)$(PREFIX)/bin

# Development dirs
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable
NAME := terminder
TARGET := $(BUILD_DIR)/$(NAME)
DEBUG_TARGET := $(BUILD_DIR)/$(NAME)_debug

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

debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SRCS)
	@mkdir -p $(@D)
	$(CXX) $(DBGFLAGS) -I$(INCLUDE_DIR) $^ -o $@

run: $(TARGET)
	./$(TARGET) $(ARGS)

install: $(TARGET)
	mkdir -p $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/

uninstall:
	rm -f $(BINDIR)$(notdir $(TARGET))

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean debug install uninstall
