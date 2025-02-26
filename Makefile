# Directories
SRCDIR := src
INCDIR := inc
BUILDDIR := build

# Compilers and flags
CC := gcc
CFLAGS := -Wall -Wextra -I$(INCDIR) -std=c11 -O2
LDFLAGS := -lm

# Source files and corresponding object files
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Final binary
TARGET := pixelans

# Default target: compile the binary
all: $(TARGET)

# Link objects to create the binary
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile each source file into its object file
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean up the build artifacts
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Declare phony targets
.PHONY: all clean
