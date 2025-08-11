#ifndef __DISJOINT_FOREST_H_
#define __DISJOINT_FOREST_H_

#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

template <class T>
struct Node {
    T data;
    Node* parent;
    int rank; // depth of the tree
};

template <class T>
class DisjointForest {
    public:
        DisjointForest();
        DisjointForest(int initial_capacity);
        ~DisjointForest();
        
        // Core operations
        Node<T>* makeSet(T data);
        Node<T>* find(Node<T>* node);
        void unionSets(Node<T>* node1, Node<T>* node2);
        
        // Dynamic operations
        void expand(int additional_capacity);
        void contract(Node<T>* node);
        void clear();
        
        // Utility methods
        int size() const;
        int capacity() const;
        bool isEmpty() const;
        std::vector<Node<T>*> getAllNodes() const;
        
    private:
        std::vector<std::unique_ptr<Node<T>>> nodes;
        std::vector<int> ranks;
        int next_id;
        
        // Helper methods
        void compressPath(Node<T>* node, Node<T>* root);
        void updateRank(Node<T>* root1, Node<T>* root2);
};

#endif // __DISJOINT_FOREST_H__