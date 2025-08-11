#include "disjoint_forest.h"
#include <algorithm>
#include <stdexcept>

template <class T>
DisjointForest<T>::DisjointForest() : next_id(0) {
    // Default constructor with no initial capacity
}

template <class T>
DisjointForest<T>::DisjointForest(int initial_capacity) : next_id(0) {
    if (initial_capacity < 0) {
        throw std::invalid_argument("Initial capacity cannot be negative");
    }
    
    // Pre-allocate space for better performance
    nodes.reserve(initial_capacity);
    ranks.reserve(initial_capacity);
}

template <class T>
DisjointForest<T>::~DisjointForest() {
    // Vectors will automatically clean up their contents
    // unique_ptr will automatically delete the Node objects
}

template <class T>
Node<T>* DisjointForest<T>::makeSet(T data) {
    // Create a new node
    auto node = std::make_unique<Node<T>>();
    node->data = data;
    node->parent = node.get();
    node->rank = 0;
    
    // Store the node and get a pointer to it
    Node<T>* node_ptr = node.get();
    nodes.push_back(std::move(node));
    ranks.push_back(0);
    
    return node_ptr;
}

template <class T>
Node<T>* DisjointForest<T>::find(Node<T>* node) {
    if (!node) {
        throw std::invalid_argument("Cannot find null node");
    }
    
    // Find the root
    Node<T>* root = node;
    while (root->parent != root) {
        root = root->parent;
    }
    
    // Path compression
    compressPath(node, root);
    
    return root;
}

template <class T>
void DisjointForest<T>::unionSets(Node<T>* node1, Node<T>* node2) {
    if (!node1 || !node2) {
        throw std::invalid_argument("Cannot union null nodes");
    }
    
    Node<T>* root1 = find(node1);
    Node<T>* root2 = find(node2);
    
    if (root1 == root2) {
        return; // Already in the same set
    }
    
    // Union by rank
    if (root1->rank > root2->rank) {
        std::swap(root1, root2);
    }
    
    root2->parent = root1;
    updateRank(root1, root2);
}

template <class T>
void DisjointForest<T>::expand(int additional_capacity) {
    if (additional_capacity <= 0) {
        throw std::invalid_argument("Additional capacity must be positive");
    }
    
    int new_capacity = nodes.capacity() + additional_capacity;
    nodes.reserve(new_capacity);
    ranks.reserve(new_capacity);
}

template <class T>
void DisjointForest<T>::contract(Node<T>* node) {
    if (!node) {
        throw std::invalid_argument("Cannot contract null node");
    }
    
    // Find all nodes that point to this node and update their parents
    Node<T>* new_parent = nullptr;
    
    // If this node is a root, find a new root among its children
    if (node->parent == node) {
        // Find a child to promote as new root
        for (auto& other_node : nodes) {
            if (other_node.get() != node && other_node->parent == node) {
                if (!new_parent) {
                    new_parent = other_node.get();
                    new_parent->parent = new_parent;
                } else {
                    other_node->parent = new_parent;
                }
            }
        }
    } else {
        // This node is not a root, so its children should point to its parent
        new_parent = node->parent;
        for (auto& other_node : nodes) {
            if (other_node.get() != node && other_node->parent == node) {
                other_node->parent = new_parent;
            }
        }
    }
    
    // Remove the node from our tracking
    auto it = std::find_if(nodes.begin(), nodes.end(),
        [node](const std::unique_ptr<Node<T>>& n) { return n.get() == node; });
    
    if (it != nodes.end()) {
        size_t index = std::distance(nodes.begin(), it);
        nodes.erase(it);
        ranks.erase(ranks.begin() + index);
    }
}

template <class T>
void DisjointForest<T>::clear() {
    nodes.clear();
    ranks.clear();
    next_id = 0;
}

template <class T>
int DisjointForest<T>::size() const {
    return static_cast<int>(nodes.size());
}

template <class T>
int DisjointForest<T>::capacity() const {
    return static_cast<int>(nodes.capacity());
}

template <class T>
bool DisjointForest<T>::isEmpty() const {
    return nodes.empty();
}

template <class T>
std::vector<Node<T>*> DisjointForest<T>::getAllNodes() const {
    std::vector<Node<T>*> result;
    result.reserve(nodes.size());
    
    for (const auto& node : nodes) {
        result.push_back(node.get());
    }
    
    return result;
}

template <class T>
void DisjointForest<T>::compressPath(Node<T>* node, Node<T>* root) {
    // Compress the path from node to root
    while (node->parent != root) {
        Node<T>* parent = node->parent;
        node->parent = root;
        node = parent;
    }
}

template <class T>
void DisjointForest<T>::updateRank(Node<T>* root1, Node<T>* root2) {
    // Update rank only if both roots have the same rank
    if (root1->rank == root2->rank) {
        root1->rank++;
    }
}

// Explicit template instantiations for the types we're using
template class DisjointForest<int>;
template class DisjointForest<std::string>;


