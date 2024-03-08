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

HEADERS := $(wildcard $(SRCDIR)/*.hpp)

# Binary name
TARGET := $(BUILDDIR)/task

# Executable name
EXECUTABLE := task

# install script
INSTALL_SCRIPT := setup.sh

# Default target
all: $(TARGET)

# Compile object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@


# setup target
install:
	chmod +x $(INSTALL_SCRIPT)
	./$(INSTALL_SCRIPT)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed $(EXECUTABLE) to /usr/local/bin/"

uninstall:
	sudo rm -f /usr/local/bin/$(EXECUTABLE)

# Clean the build directory
clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
