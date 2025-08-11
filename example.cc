#include "disjoint_forest.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "DisjointForest Example\n";
    std::cout << "======================\n\n";
    
    // Create a disjoint forest with initial capacity for 5 elements
    DisjointForest<int> forest(5);
    
    std::cout << "Initial capacity: " << forest.capacity() << std::endl;
    std::cout << "Initial size: " << forest.size() << std::endl;
    std::cout << "Is empty: " << (forest.isEmpty() ? "Yes" : "No") << std::endl << std::endl;
    
    // Create some sets
    std::vector<Node<int>*> nodes;
    for (int i = 0; i < 10; i++) {
        nodes.push_back(forest.makeSet(i));
        std::cout << "Created set with element: " << i << std::endl;
    }
    
    std::cout << "\nAfter creating 10 nodes:" << std::endl;
    std::cout << "Size: " << forest.size() << std::endl;
    std::cout << "Capacity: " << forest.capacity() << std::endl << std::endl;
    
    // Expand capacity
    std::cout << "Expanding capacity by 20..." << std::endl;
    forest.expand(20);
    std::cout << "New capacity: " << forest.capacity() << std::endl << std::endl;
    
    std::cout << "Performing unions...\n";
    
    // Union even numbers
    for (int i = 0; i < 8; i += 2) {
        forest.unionSets(nodes[i], nodes[i + 2]);
        std::cout << "United " << i << " and " << (i + 2) << std::endl;
    }
    
    // Union odd numbers
    for (int i = 1; i < 9; i += 2) {
        forest.unionSets(nodes[i], nodes[i + 2]);
        std::cout << "United " << i << " and " << (i + 2) << std::endl;
    }
    
    // Union 0 and 1 to connect even and odd groups
    forest.unionSets(nodes[0], nodes[1]);
    std::cout << "United 0 and 1\n";
    
    std::cout << "\nFinding representatives...\n";
    
    // Find representatives for all elements
    for (int i = 0; i < 10; i++) {
        Node<int>* representative = forest.find(nodes[i]);
        std::cout << "Element " << i << " belongs to set with representative: " 
                  << representative->data << std::endl;
    }
    
    std::cout << "\nAll elements are now in the same set!\n";
    
    // Demonstrate contraction
    std::cout << "\nDemonstrating contraction...\n";
    std::cout << "Size before contraction: " << forest.size() << std::endl;
    
    // Contract some nodes
    forest.contract(nodes[5]);
    forest.contract(nodes[7]);
    std::cout << "Contracted nodes 5 and 7\n";
    std::cout << "Size after contraction: " << forest.size() << std::endl;
    
    // Show that remaining nodes are still connected
    std::cout << "\nChecking connectivity after contraction...\n";
    // Check that all elements are in the same set
    forest.find(nodes[0]);
    for (int i = 0; i < 10; i++) {
        if (i != 5 && i != 7) {
            Node<int>* representative = forest.find(nodes[i]);
            std::cout << "Element " << i << " representative: " << representative->data << std::endl;
        }
    }
    
    // Demonstrate getAllNodes
    std::cout << "\nGetting all remaining nodes...\n";
    std::vector<Node<int>*> all_nodes = forest.getAllNodes();
    std::cout << "Number of remaining nodes: " << all_nodes.size() << std::endl;
    
    // Demonstrate clear
    std::cout << "\nClearing the forest...\n";
    forest.clear();
    std::cout << "Size after clear: " << forest.size() << std::endl;
    std::cout << "Is empty: " << (forest.isEmpty() ? "Yes" : "No") << std::endl;
    
    return 0;
} 