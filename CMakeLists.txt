cmake_minimum_required(VERSION 3.12)
project(HuffmanCLI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Define our executable and the source files it requires
add_executable(huffman_cli 
    src/main.cpp 
    src/huffman.cpp
)