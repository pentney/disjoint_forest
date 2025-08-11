# Makefile for DisjointForest project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TESTFLAGS = -lgtest_main -lgtest -lpthread

# Directories
SRCDIR = .
BUILDDIR = build
TESTDIR = .

# Source files
SOURCES = $(SRCDIR)/disjoint_forest.cc
TEST_SOURCES = $(TESTDIR)/disjoint_forest_test.cc
EXAMPLE_SOURCES = $(TESTDIR)/example.cc

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.cc=$(BUILDDIR)/%.o)
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:$(TESTDIR)/%.cc=$(BUILDDIR)/%.o)

# Executables
TEST_EXEC = disjoint_forest_test
EXAMPLE_EXEC = disjoint_forest_example
LIBRARY = libdisjoint_forest.a

# Check if gtest is available and can be compiled against
GTEST_AVAILABLE := $(shell g++ -std=c++17 -c -x c++ /dev/null -o /dev/null 2>/dev/null && echo "compiler_ok" && g++ -std=c++17 -lgtest_main -lgtest -lpthread -x c++ /dev/null -o /dev/null 2>/dev/null && echo "gtest_ok" || echo "not_ok")

# Default target
all: ensure-gtest $(BUILDDIR) $(TEST_EXEC) $(EXAMPLE_EXEC) python-bindings

# Ensure gtest is available before building
ensure-gtest:
	@echo "Checking Google Test availability..."
	@if [ "$(GTEST_AVAILABLE)" != "gtest_ok" ]; then \
		echo "Google Test not found or not working. Installing..."; \
		$(MAKE) install-gtest; \
		echo "Verifying installation..."; \
		$(MAKE) verify-gtest; \
	else \
		echo "Google Test is available and working."; \
	fi

# Verify gtest installation
verify-gtest:
	@echo "Verifying Google Test installation..."
	@if g++ -std=c++17 -lgtest_main -lgtest -lpthread -x c++ /dev/null -o /dev/null 2>/dev/null; then \
		echo "Google Test verification successful!"; \
	else \
		echo "Google Test verification failed. Trying alternative linking order..."; \
		if g++ -std=c++17 -lgtest_main -lgtest -lpthread -x c++ /dev/null -o /dev/null 2>/dev/null; then \
			echo "Google Test verification successful with alternative linking order!"; \
			echo "Updating Makefile with correct linking order..."; \
			sed -i 's/-lgtest_main -lgtest -lpthread/-lgtest_main -lgtest -lpthread/g' Makefile; \
		else \
			echo "Google Test verification failed. Please install manually:"; \
			echo "  Ubuntu/Debian: sudo apt-get install libgtest-dev"; \
			echo "  macOS: brew install googletest"; \
			echo "  CentOS/RHEL: sudo yum install gtest-devel"; \
			echo "  Arch: sudo pacman -S gtest"; \
			exit 1; \
		fi; \
	fi

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Build the library
$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

# Build test executable
$(TEST_EXEC): $(TEST_OBJECTS) $(LIBRARY)
	@echo "Building test executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TESTFLAGS)

# Build example executable
$(EXAMPLE_EXEC): $(EXAMPLE_OBJECTS) $(LIBRARY)
	@echo "Building example executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TESTFLAGS)

# Compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(BUILDDIR)/%.o: $(TESTDIR)/%.cc
	@echo "Compiling test file: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Run example
example: $(EXAMPLE_EXEC)
	./$(EXAMPLE_EXEC)

# Build Python bindings
python-bindings:
	@echo "Building Python bindings..."
	$(MAKE) -C python build

# Run Python example
python-example: python-bindings
	@echo "Running Python example..."
	$(MAKE) -C python example

# Test Python bindings
python-test: python-bindings
	@echo "Testing Python bindings..."
	$(MAKE) -C python test

# Clean build files
clean:
	rm -rf $(BUILDDIR) $(TEST_EXEC) $(EXAMPLE_EXEC) $(LIBRARY)
	$(MAKE) -C python clean

# Install Google Test (Ubuntu/Debian)
install-gtest:
	@echo "Installing Google Test..."
	@if command -v apt-get >/dev/null 2>&1; then \
		echo "Using apt-get to install Google Test..."; \
		sudo apt-get update && sudo apt-get install -y libgtest-dev; \
		echo "Google Test installed successfully!"; \
	elif command -v brew >/dev/null 2>&1; then \
		echo "Using Homebrew to install Google Test..."; \
		brew install googletest; \
		echo "Google Test installed successfully!"; \
	elif command -v yum >/dev/null 2>&1; then \
		echo "Using yum to install Google Test..."; \
		sudo yum install -y gtest-devel; \
		echo "Google Test installed successfully!"; \
	elif command -v pacman >/dev/null 2>&1; then \
		echo "Using pacman to install Google Test..."; \
		sudo pacman -S --noconfirm gtest; \
		echo "Google Test installed successfully!"; \
	else \
		echo "No supported package manager found. Please install Google Test manually:"; \
		echo "  Ubuntu/Debian: sudo apt-get install libgtest-dev"; \
		echo "  macOS: brew install googletest"; \
		echo "  CentOS/RHEL: sudo yum install gtest-devel"; \
		echo "  Arch: sudo pacman -S gtest"; \
		exit 1; \
	fi

# Install Google Test (Ubuntu/Debian) - legacy target
install-gtest-ubuntu:
	sudo apt-get update
	sudo apt-get install -y libgtest-dev

# Install Google Test (macOS) - legacy target
install-gtest-mac:
	brew install googletest

# Install Google Test (CentOS/RHEL)
install-gtest-centos:
	sudo yum install -y gtest-devel

# Install Google Test (Arch)
install-gtest-arch:
	sudo pacman -S --noconfirm gtest

# Check system info
system-info:
	@echo "System Information:"
	@echo "  OS: $(shell uname -s)"
	@echo "  Architecture: $(shell uname -m)"
	@echo "  Package managers available:"
	@if command -v apt-get >/dev/null 2>&1; then echo "    - apt-get (Ubuntu/Debian)"; fi
	@if command -v brew >/dev/null 2>&1; then echo "    - Homebrew (macOS)"; fi
	@if command -v yum >/dev/null 2>&1; then echo "    - yum (CentOS/RHEL)"; fi
	@if command -v pacman >/dev/null 2>&1; then echo "    - pacman (Arch)"; fi
	@echo "  Google Test available: $(GTEST_AVAILABLE)"

# Show help
help:
	@echo "Available targets:"
	@echo "  all              - Build everything including Python bindings (default)"
	@echo "  test             - Build and run tests"
	@echo "  example          - Build and run example"
	@echo "  python-bindings  - Build Python bindings"
	@echo "  python-example   - Build and run Python example"
	@echo "  python-test      - Build and test Python bindings"
	@echo "  clean            - Remove build files"
	@echo "  install-gtest    - Install Google Test automatically"
	@echo "  verify-gtest     - Verify Google Test installation"
	@echo "  system-info      - Show system information and available package managers"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "The 'all' target will automatically check for and install Google Test if needed."

.PHONY: all test example clean install-gtest install-gtest-ubuntu install-gtest-mac install-gtest-centos install-gtest-arch system-info help ensure-gtest verify-gtest python-bindings python-example python-test 