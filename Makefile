# compiler flags
CXX := g++
CXXFLAGS := -pthread -std=c++17 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Iinclude

# Directories
SRCDIR := src
BUILDDIR := build
INCLUDEDIR := include
#
# Files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Target executable
TARGET := superFrmwrk

# Rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean
