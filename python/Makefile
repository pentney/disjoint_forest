# Makefile for Python bindings

PYTHON = python3
PIP = pip3

# Default target
all: install-deps build

# Install Python dependencies
install-deps:
	@echo "Installing Python dependencies..."
	$(PIP) install -r requirements.txt

# Build the Python module
build: install-deps
	@echo "Building Python bindings..."
	$(PYTHON) setup.py build_ext --inplace

# Clean build artifacts
clean:
	@echo "Cleaning Python build artifacts..."
	rm -rf build/ *.so *.egg-info/ __pycache__/

# Install the module
install: build
	@echo "Installing Python module..."
	$(PIP) install -e .

# Run the Python example
example: build
	@echo "Running Python example..."
	$(PYTHON) example.py

# Test the bindings
test: build
	@echo "Testing Python bindings..."
	$(PYTHON) -c "import disjoint_forest; print('Import successful!')"

# Test package structure
package-test: build
	@echo "Testing package structure..."
	$(PYTHON) test_package.py

# Create distribution packages
dist: build
	@echo "Creating distribution packages..."
	$(PYTHON) setup.py sdist
	$(PYTHON) setup.py bdist_wheel --plat-name manylinux2014_x86_64

# Show help
help:
	@echo "Available targets:"
	@echo "  all              - Install deps and build (default)"
	@echo "  install-deps     - Install Python dependencies"
	@echo "  build            - Build the Python module"
	@echo "  clean            - Clean build artifacts"
	@echo "  install          - Install the module"
	@echo "  example          - Run the Python example"
	@echo "  test             - Test the bindings"
	@echo "  package-test     - Test package structure"
	@echo "  dist             - Create distribution packages"
	@echo "  help             - Show this help message"

.PHONY: all install-deps build clean install example test package-test dist help 