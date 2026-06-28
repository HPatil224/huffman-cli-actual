# Huffman CLI File Compressor

A high-performance, zero-dependency command-line utility built in modern C++ (C++17) that performs lossless data compression. This project implements the classic Huffman Coding algorithm to significantly reduce text file sizes by encoding frequent characters with optimal variable-length bit sequences.

## Features
* **Lossless Compression:** Guarantees 100% data retention. The decompressed file is an exact byte-for-byte replica of the original.
* **Bit-Level Optimization:** Custom bit-packing logic seamlessly packs generated binary string codes directly into raw 8-bit memory buffers, heavily optimizing disk I/O.
* **Algorithmically Efficient:** Utilizes recursive Depth-First Search (DFS) for rapid tree traversal and code generation, processing file streams with an overall time complexity of **O(N log K)**.
* **Zero Dependencies:** Built entirely with pure standard C++ (STL). No external frameworks required.

## Tech Stack
* **Language:** C++17
* **Core Libraries:** C++ Standard Template Library (STL) - `<unordered_map>`, `<queue>`, `<fstream>`
* **Concepts:** Min-Heaps (Priority Queues), Binary Trees, Hash Maps, Bitwise Operations, Binary File I/O

## Under the Hood (How it Works)
This compressor executes through four highly optimized phases:
1. **Frequency Mapping:** Reads the target file via binary streams and uses a Hash Map to count character occurrences accurately.
2. **Tree Construction:** Pushes character nodes into a Min-Heap and iteratively merges the lowest-frequency nodes to dynamically build an optimal Huffman Tree.
3. **Encoding & Bit-Packing:** Traverses the tree using DFS to generate custom prefix codes, then uses bitwise shift (`<<`) and masking (`|`) operators to pack these binary codes into memory.
4. **Decompression:** Reads the compressed `.huf` binary file bit-by-bit, traversing the reconstructed tree to decode and seamlessly restore the original text.

## Getting Started

### Prerequisites
* A C++ compiler that supports C++17 (e.g., GCC/G++).

### Build Instructions
Navigate to the root directory of the project and run the following command to compile the source code:

```bash
g++ -std=c++17 -I include src/main.cpp src/huffman.cpp -o huffman_cli
