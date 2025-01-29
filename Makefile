# Compiler and linker flags
CXX := g++
CXXFLAGS := -pthread -std=c++17 -Wall -Wextra -Wshadow -Wpessimizing-move -Wnon-virtual-dtor -pedantic -Iinclude -I/usr/include/brotli
LDFLAGS := -L/usr/lib64 -lbrotlienc -lbrotlicommon

# Directories
SRCDIR := src
BUILDDIR := build
INCLUDEDIR := include
DOCDIR := docs

# Files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Target executable
TARGET := superFrmwrk

# Rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)  # Explicitly include LDFLAGS here

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

docs:
	doxygen Doxyfile > /dev/null

.PHONY: all clean docs
