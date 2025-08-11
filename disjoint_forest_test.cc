#include <gtest/gtest.h>
#include "disjoint_forest.h"
#include <vector>
#include <memory>

// Test fixture for DisjointForest tests
template<typename T>
class DisjointForestTest : public ::testing::Test {
protected:
    void SetUp() override {
        forest = std::make_unique<DisjointForest<T>>();
    }
    
    void TearDown() override {
        forest.reset();
    }
    
    std::unique_ptr<DisjointForest<T>> forest;
};

// Test with int type
using DisjointForestIntTest = DisjointForestTest<int>;

// Test with string type
using DisjointForestStringTest = DisjointForestTest<std::string>;

// Test basic constructor and destructor
TEST_F(DisjointForestIntTest, ConstructorDestructor) {
    DisjointForest<int> localForest;
    DisjointForest<int> localForestWithCapacity(10);
    // Should not crash on destruction
}

// Test makeSet functionality
TEST_F(DisjointForestIntTest, MakeSet) {
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    
    ASSERT_NE(node1, nullptr);
    ASSERT_NE(node2, nullptr);
    ASSERT_NE(node1, node2);
    
    EXPECT_EQ(node1->data, 1);
    EXPECT_EQ(node2->data, 2);
    EXPECT_EQ(node1->parent, node1);
    EXPECT_EQ(node2->parent, node2);
    EXPECT_EQ(node1->rank, 0);
    EXPECT_EQ(node2->rank, 0);
    
    EXPECT_EQ(forest->size(), 2);
    EXPECT_FALSE(forest->isEmpty());
}

// Test find functionality on single nodes
TEST_F(DisjointForestIntTest, FindSingleNode) {
    Node<int>* node = forest->makeSet(42);
    Node<int>* found = forest->find(node);
    
    EXPECT_EQ(found, node);
    EXPECT_EQ(found->data, 42);
}

// Test find functionality with path compression
TEST_F(DisjointForestIntTest, FindWithPathCompression) {
    // Create a chain: 1 -> 2 -> 3 -> 4
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    Node<int>* node3 = forest->makeSet(3);
    Node<int>* node4 = forest->makeSet(4);
    
    // Manually create the chain
    node2->parent = node1;
    node3->parent = node2;
    node4->parent = node3;
    
    // Find should return root and compress path
    Node<int>* found = forest->find(node4);
    EXPECT_EQ(found, node1);
    
    // Path should be compressed
    EXPECT_EQ(node4->parent, node1);
    EXPECT_EQ(node3->parent, node1);
    EXPECT_EQ(node2->parent, node1);
}

// Test unionSets functionality
TEST_F(DisjointForestIntTest, UnionSets) {
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    
    forest->unionSets(node1, node2);
    
    // Both should now have the same root
    Node<int>* root1 = forest->find(node1);
    Node<int>* root2 = forest->find(node2);
    EXPECT_EQ(root1, root2);
}

// Test unionSets with rank optimization
TEST_F(DisjointForestIntTest, UnionSetsWithRank) {
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    Node<int>* node3 = forest->makeSet(3);
    
    // Union 1 and 2, then union with 3
    forest->unionSets(node1, node2);
    forest->unionSets(node1, node3);
    
    Node<int>* root1 = forest->find(node1);
    Node<int>* root2 = forest->find(node2);
    Node<int>* root3 = forest->find(node3);
    
    EXPECT_EQ(root1, root2);
    EXPECT_EQ(root2, root3);
    EXPECT_EQ(root1, root3);
}

// Test unionSets with same set
TEST_F(DisjointForestIntTest, UnionSetsSameSet) {
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    
    // First union
    forest->unionSets(node1, node2);
    
    // Union again - should not change anything
    forest->unionSets(node1, node2);
    
    Node<int>* root1 = forest->find(node1);
    Node<int>* root2 = forest->find(node2);
    EXPECT_EQ(root1, root2);
}

// Test expand functionality
TEST_F(DisjointForestIntTest, Expand) {
    int initial_capacity = forest->capacity();
    
    // Add some nodes to see capacity change
    for (int i = 0; i < 10; i++) {
        forest->makeSet(i);
    }
    
    // Expand capacity
    forest->expand(50);
    int new_capacity = forest->capacity();
    
    EXPECT_GE(new_capacity, initial_capacity + 50);
    EXPECT_EQ(forest->size(), 10); // Size should remain the same
}

// Test contract functionality
TEST_F(DisjointForestIntTest, Contract) {
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    Node<int>* node3 = forest->makeSet(3);
    
    // Union 1 and 2
    forest->unionSets(node1, node2);
    
    int size_before = forest->size();
    
    // Contract node2 (which is not a root)
    forest->contract(node2);
    
    EXPECT_EQ(forest->size(), size_before - 1);
    
    // node1 and node3 should still exist
    EXPECT_EQ(forest->find(node1), node1);
    EXPECT_EQ(forest->find(node3), node3);
}

// Test contract root node
TEST_F(DisjointForestIntTest, ContractRoot) {
    Node<int>* node1 = forest->makeSet(1);
    Node<int>* node2 = forest->makeSet(2);
    Node<int>* node3 = forest->makeSet(3);
    
    // Union all three
    forest->unionSets(node1, node2);
    forest->unionSets(node1, node3);
    
    int size_before = forest->size();
    
    // Contract the root (node1)
    forest->contract(node1);
    
    EXPECT_EQ(forest->size(), size_before - 1);
    
    // node2 and node3 should still be connected
    EXPECT_EQ(forest->find(node2), forest->find(node3));
}

// Test clear functionality
TEST_F(DisjointForestIntTest, Clear) {
    // Add some nodes
    for (int i = 0; i < 5; i++) {
        forest->makeSet(i);
    }
    
    EXPECT_EQ(forest->size(), 5);
    EXPECT_FALSE(forest->isEmpty());
    
    forest->clear();
    
    EXPECT_EQ(forest->size(), 0);
    EXPECT_TRUE(forest->isEmpty());
}

// Test getAllNodes functionality
TEST_F(DisjointForestIntTest, GetAllNodes) {
    std::vector<Node<int>*> created_nodes;
    
    // Create some nodes
    for (int i = 0; i < 5; i++) {
        created_nodes.push_back(forest->makeSet(i));
    }
    
    std::vector<Node<int>*> all_nodes = forest->getAllNodes();
    
    EXPECT_EQ(all_nodes.size(), created_nodes.size());
    
    // Check that all created nodes are in the result
    for (auto node : created_nodes) {
        auto it = std::find(all_nodes.begin(), all_nodes.end(), node);
        EXPECT_NE(it, all_nodes.end());
    }
}

// Test complex scenario with dynamic operations
TEST_F(DisjointForestIntTest, ComplexDynamicScenario) {
    std::vector<Node<int>*> nodes;
    
    // Create 10 nodes
    for (int i = 0; i < 10; i++) {
        nodes.push_back(forest->makeSet(i));
    }
    
    // Union even numbers
    for (int i = 0; i < 8; i += 2) {
        forest->unionSets(nodes[i], nodes[i + 2]);
    }
    
    // Union odd numbers
    for (int i = 1; i < 9; i += 2) {
        forest->unionSets(nodes[i], nodes[i + 2]);
    }
    
    // Union 0 and 1
    forest->unionSets(nodes[0], nodes[1]);
    
    // All nodes should now be in the same set
    Node<int>* root = forest->find(nodes[0]);
    for (auto node : nodes) {
        EXPECT_EQ(forest->find(node), root);
    }
    
    // Now contract some nodes
    forest->contract(nodes[5]);
    forest->contract(nodes[7]);
    
    EXPECT_EQ(forest->size(), 8);
    
    // Remaining nodes should still be connected
    Node<int>* new_root = forest->find(nodes[0]);
    for (int i = 0; i < 10; i++) {
        if (i != 5 && i != 7) {
            EXPECT_EQ(forest->find(nodes[i]), new_root);
        }
    }
}

// Test with string type
TEST_F(DisjointForestStringTest, StringOperations) {
    Node<std::string>* node1 = forest->makeSet("hello");
    Node<std::string>* node2 = forest->makeSet("world");
    
    EXPECT_EQ(node1->data, "hello");
    EXPECT_EQ(node2->data, "world");
    
    forest->unionSets(node1, node2);
    
    Node<std::string>* root1 = forest->find(node1);
    Node<std::string>* root2 = forest->find(node2);
    EXPECT_EQ(root1, root2);
}

// Test edge cases
TEST_F(DisjointForestIntTest, EdgeCases) {
    // Test with size 0
    EXPECT_TRUE(forest->isEmpty());
    EXPECT_EQ(forest->size(), 0);
    
    // Test expand with invalid capacity
    EXPECT_THROW(forest->expand(-5), std::invalid_argument);
    EXPECT_THROW(forest->expand(0), std::invalid_argument);
}

// Test error handling
TEST_F(DisjointForestIntTest, ErrorHandling) {
    // Test find with nullptr
    EXPECT_THROW(forest->find(nullptr), std::invalid_argument);
    
    // Test unionSets with nullptr
    Node<int>* node = forest->makeSet(1);
    EXPECT_THROW(forest->unionSets(nullptr, node), std::invalid_argument);
    EXPECT_THROW(forest->unionSets(node, nullptr), std::invalid_argument);
    
    // Test contract with nullptr
    EXPECT_THROW(forest->contract(nullptr), std::invalid_argument);
}

// Test memory management
TEST_F(DisjointForestIntTest, MemoryManagement) {
    // Create many nodes and perform operations
    std::vector<Node<int>*> nodes;
    for (int i = 0; i < 50; i++) {
        nodes.push_back(forest->makeSet(i));
    }
    
    // Perform many unions
    for (int i = 0; i < 49; i++) {
        forest->unionSets(nodes[i], nodes[i + 1]);
    }
    
    // All should be in same set
    Node<int>* root = forest->find(nodes[0]);
    for (auto node : nodes) {
        EXPECT_EQ(forest->find(node), root);
    }
    
    // Contract some nodes
    for (int i = 10; i < 20; i++) {
        forest->contract(nodes[i]);
    }
    
    EXPECT_EQ(forest->size(), 40);
    
    // Should not crash on destruction
}

// Main function to run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 