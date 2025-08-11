SHELL ?= /bin/sh
CXX ?= clang++
CXXFLAGS ?= -std=c++23
RELEASEFLAGS := -O2 $(CXXFLAGS)
DBGFLAGS := -g -Og -Wall -Wextra -pedantic $(CXXFLAGS)

# Output dirs
prefix ?= /usr/local
prefix ?=
bindir = $(prefix)/bin
DESTDIR ?=

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

install: $(TARGET)
	@mkdir -p $(DESTDIR)$(bindir)
	install -m 0755 $(TARGET) $(bindir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/$(notdir $(TARGET))

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean debug install uninstall
