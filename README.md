# DisjointForest

A C++ implementation of a disjoint-set data structure (also known as union-find) using the disjoint forest representation with path compression and union by rank optimizations, with Python bindings included as well. This is intended to fill a void in too many existing standard libraries, which don't include a proper implementation of this despite its utility in many applications.

## Features

- **Template-based**: Works with any data type
- **Path Compression**: Automatically compresses paths during find operations
- **Union by Rank**: Optimizes union operations by maintaining tree depth information
- **Dynamic Operations**: Expand capacity, contract nodes, and clear the forest
- **Memory Efficient**: STL vectors with automatic memory management
- **Comprehensive Testing**: Full unit test suite using Google Test

## Class Structure

### Node<T>
- `data`: The actual data stored in the node
- `parent`: Pointer to the parent node (self-referential for root nodes)
- `rank`: Depth of the subtree rooted at this node

### DisjointForest<T>
- `makeSet(T data)`: Creates a new set containing the given data
- `find(Node<T>* node)`: Finds the representative of the set containing the node
- `unionSets(Node<T>* node1, Node<T>* node2)`: Merges two sets
- Constructor takes the maximum number of elements as parameter

## Building the Project

### Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler
- Google Test library

### Install Google Test (Ubuntu/Debian)
```bash
sudo apt-get install libgtest-dev
```

### Install Google Test (macOS)
```bash
brew install googletest
```

### Build Steps
```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run tests
make test
# or directly
./disjoint_forest_test

# Run example
./disjoint_forest_example
```

## Running Tests

The test suite covers:

1. **Basic Operations**: Constructor, destructor, makeSet
2. **Find Operations**: Single node find, path compression
3. **Union Operations**: Basic union, union by rank, same set union
4. **Complex Scenarios**: Multiple unions, large datasets
5. **Edge Cases**: Empty forests, single element forests
6. **Memory Management**: Large operations, proper cleanup
7. **Template Support**: Tests with different data types (int, string)

## Example Usage

```cpp
#include "disjoint_forest.h"

// Create a forest with capacity for 100 elements
DisjointForest<int> forest(100);

// Create some sets
Node<int>* node1 = forest.makeSet(1);
Node<int>* node2 = forest.makeSet(2);
Node<int>* node3 = forest.makeSet(3);

// Union sets
forest.unionSets(node1, node2);
forest.unionSets(node1, node3);

// Find representatives
Node<int>* rep1 = forest.find(node1);
Node<int>* rep2 = forest.find(node2);
Node<int>* rep3 = forest.find(node3);

// All should have the same representative
assert(rep1 == rep2 && rep2 == rep3);
```

## Algorithm Complexity

- **makeSet**: O(1)
- **find**: O(α(n)) amortized, where α is the inverse Ackermann function
- **unionSets**: O(α(n)) amortized
- **Space**: O(n) where n is the number of elements

## Notes

- The implementation automatically handles path compression during find operations
- Union by rank ensures balanced trees for optimal performance
- Memory is automatically managed with proper cleanup in the destructor

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the implementation or test coverage. 
