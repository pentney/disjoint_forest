cmake_minimum_required(VERSION 3.10)
project(DisjointForest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Google Test
find_package(GTest REQUIRED)

# Create the main library
add_library(disjoint_forest disjoint_forest.cc)

# Create the test executable
add_executable(disjoint_forest_test disjoint_forest_test.cc)

# Link the test executable with the library and Google Test
target_link_libraries(disjoint_forest_test 
    disjoint_forest 
    GTest::GTest 
    GTest::Main
)

# Enable testing
enable_testing()
add_test(NAME DisjointForestTests COMMAND disjoint_forest_test)

# Set compiler flags for better debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(disjoint_forest_test PRIVATE -g -O0 -Wall -Wextra)
    target_compile_options(disjoint_forest PRIVATE -g -O0 -Wall -Wextra)
else()
    target_compile_options(disjoint_forest_test PRIVATE -O2 -Wall -Wextra)
    target_compile_options(disjoint_forest PRIVATE -O2 -Wall -Wextra)
endif()

# Create a simple example executable
add_executable(disjoint_forest_example example.cc)
target_link_libraries(disjoint_forest_example disjoint_forest) 