SHELL ?= /bin/sh
CXX ?= clang++

# Build type from environment variable (default: release)
BUILD_TYPE ?= release

# Base flags
BASE_CXXFLAGS := -std=c++23

# Conditional flags based on BUILD_TYPE
ifeq ($(BUILD_TYPE),debug)
    CXXFLAGS := $(BASE_CXXFLAGS) -g -Og -Wall -Wextra -pedantic
else
    CXXFLAGS := $(BASE_CXXFLAGS) -O2
endif

# Output dirs
prefix ?= /usr/local
prefix ?=
bindir = $(prefix)/bin
DESTDIR ?=

# Development dirs
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
PROJECT_FILE := $(BUILD_DIR)/compile_commands.json

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

NAME := terminder

ifeq ($(BUILD_TYPE),debug)
	OBJ_DIR := $(BUILD_DIR)/obj_debug
    TARGET := $(BUILD_DIR)/$(NAME)_debug
else
	OBJ_DIR := $(BUILD_DIR)/obj
    TARGET := $(BUILD_DIR)/$(NAME)
endif

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

setup:
	@rm -rf $(BUILD_DIR)
	@mkdir $(BUILD_DIR)
	bear --output $(PROJECT_FILE) -- $(MAKE)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

install: $(TARGET)
	@mkdir -p $(DESTDIR)$(bindir)
	install -m 0755 $(TARGET) $(bindir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/$(notdir $(TARGET))

clean:
	rm -rf $(BUILD_DIR)

format:
	clang-format -i $(SRCS) $(wildcard $(INCLUDE_DIR)/*.hpp)

tidy:
	run-clang-tidy -p $(BUILD_DIR) -fix -format

tidy-ci:
	run-clang-tidy -p $(BUILD_DIR)

check:
	cppcheck -I $(INCLUDE_DIR) --project=$(PROJECT_FILE)

.PHONY: all clean install uninstall format setup check
