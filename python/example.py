#!/usr/bin/env python3
"""
Python example demonstrating the DisjointForest bindings
"""

import disjoint_forest

def main():
    print("DisjointForest Python Example")
    print("=" * 40)

    # Create a forest with initial capacity
    forest = disjoint_forest.DisjointForest(5)
    print(f"Initial capacity: {forest.capacity()}")
    print(f"Initial size: {forest.size()}")
    print(f"Is empty: {forest.is_empty()}")
    print()

    # Create some sets with integers
    nodes = []
    for i in range(10):
        node = forest.make_set(i)
        nodes.append(node)
        print(f"Created set with element: {node.data}")

    print(f"\nAfter creating 10 nodes:")
    print(f"Size: {forest.size()}")
    print(f"Capacity: {forest.capacity()}")

    # Expand capacity
    print(f"\nExpanding capacity by 20...")
    forest.expand(20)
    print(f"New capacity: {forest.capacity()}")

    # Perform unions
    print(f"\nPerforming unions...")
    forest.union_sets(nodes[0], nodes[2])
    print(f"United {nodes[0].data} and {nodes[2].data}")

    forest.union_sets(nodes[2], nodes[4])
    print(f"United {nodes[2].data} and {nodes[4].data}")

    forest.union_sets(nodes[4], nodes[6])
    print(f"United {nodes[4].data} and {nodes[6].data}")

    forest.union_sets(nodes[6], nodes[8])
    print(f"United {nodes[6].data} and {nodes[8].data}")

    forest.union_sets(nodes[1], nodes[3])
    print(f"United {nodes[1].data} and {nodes[3].data}")

    forest.union_sets(nodes[3], nodes[5])
    print(f"United {nodes[3].data} and {nodes[5].data}")

    forest.union_sets(nodes[5], nodes[7])
    print(f"United {nodes[5].data} and {nodes[7].data}")

    forest.union_sets(nodes[7], nodes[9])
    print(f"United {nodes[7].data} and {nodes[9].data}")

    # Final union to connect all sets
    forest.union_sets(nodes[0], nodes[1])
    print(f"United {nodes[0].data} and {nodes[1].data}")

    # Find representatives
    print(f"\nFinding representatives...")
    for i in range(10):
        rep = forest.find(nodes[i])
        print(f"Element {nodes[i].data} belongs to set with representative: {rep.data}")

    print(f"\nAll elements are now in the same set!")

    # Demonstrate contraction
    print(f"\nDemonstrating contraction...")
    print(f"Size before contraction: {forest.size()}")

    forest.contract(nodes[5])
    print(f"Contracted node {nodes[5].data}")

    forest.contract(nodes[7])
    print(f"Contracted node {nodes[7].data}")

    print(f"Size after contraction: {forest.size()}")

    # Check connectivity after contraction
    print(f"\nChecking connectivity after contraction...")
    remaining_nodes = [nodes[i] for i in range(10) if i not in [5, 7]]
    for node in remaining_nodes:
        rep = forest.find(node)
        print(f"Element {node.data} representative: {rep.data}")

    # Get all remaining nodes
    all_nodes = forest.get_all_nodes()
    print(f"\nGetting all remaining nodes...")
    print(f"Number of remaining nodes: {len(all_nodes)}")

    # Clear the forest
    print(f"\nClearing the forest...")
    forest.clear()
    print(f"Size after clear: {forest.size()}")
    print(f"Is empty: {forest.is_empty()}")

    # String example
    print(f"\n" + "=" * 40)
    print("String DisjointForest Example")
    print("=" * 40)

    str_forest = disjoint_forest.DisjointForest()

    # Create string sets
    str_nodes = []
    cities = ["New York", "Boston", "Chicago", "Los Angeles", "San Francisco"]

    for city in cities:
        node = str_forest.make_set(city)
        str_nodes.append(node)
        print(f"Created set with city: {node.data}")

    # Union some cities
    str_forest.union_sets(str_nodes[0], str_nodes[1])  # NY + Boston
    str_forest.union_sets(str_nodes[2], str_nodes[3])  # Chicago + LA

    print(f"\nAfter unions:")
    for node in str_nodes:
        rep = str_forest.find(node)
        print(f"{node.data} -> {rep.data}")

    print(f"\nForest size: {str_forest.size()}")

    # Mixed types example
    print(f"\n" + "=" * 40)
    print("Mixed Types Example")
    print("=" * 40)

    mixed_forest = disjoint_forest.DisjointForest()

    # Create sets with different types
    mixed_nodes = []
    mixed_data = [42, "hello", 3.14, [1, 2, 3], {"key": "value"}]

    for data in mixed_data:
        node = mixed_forest.make_set(data)
        mixed_nodes.append(node)
        print(f"Created set with data: {data} (type: {type(data).__name__})")

    # Union some sets
    mixed_forest.union_sets(mixed_nodes[0], mixed_nodes[2])  # 42 + 3.14
    mixed_forest.union_sets(mixed_nodes[1], mixed_nodes[3])  # "hello" + [1,2,3]

    print(f"\nAfter unions:")
    for node in mixed_nodes:
        rep = mixed_forest.find(node)
        print(f"{node.data} -> {rep.data}")

    print(f"\nMixed forest size: {mixed_forest.size()}")

    # Union operator example
    print(f"\n" + "=" * 40)
    print("Union Operator Example")
    print("=" * 40)

    # Create two separate forests
    forest_a = disjoint_forest.DisjointForest(3)
    forest_b = disjoint_forest.DisjointForest(2)

    # Add some data to forest A
    node_a1 = forest_a.make_set("Apple")
    node_a2 = forest_a.make_set("Banana")
    forest_a.union_sets(node_a1, node_a2)

    # Add some data to forest B
    node_b1 = forest_b.make_set("Cherry")
    node_b2 = forest_b.make_set("Date")

    print(f"Forest A: {forest_a.get_all_data()}")
    print(f"Forest B: {forest_b.get_all_data()}")

    # Use the | operator to create a union
    union_forest = forest_a | forest_b
    print(f"Union forest (A | B): {union_forest.get_all_data()}")
    print(f"Union forest size: {union_forest.size()}")

    # Use the |= operator for in-place union
    forest_a |= forest_b
    print(f"Forest A after |= B: {forest_a.get_all_data()}")
    print(f"Forest A size after |=: {forest_a.size()}")

    # Test with different data types
    type_forest = disjoint_forest.DisjointForest(2)
    type_forest.make_set(100)
    type_forest.make_set("text")

    mixed_union = union_forest | type_forest
    print(f"Mixed type union: {mixed_union.get_all_data()}")

if __name__ == "__main__":
    main() 