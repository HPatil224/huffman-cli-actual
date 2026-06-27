#include "huffman.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

// Phase 0: Read the file and count character frequencies
std::unordered_map<char, int> buildFrequencyMap(const std::string& filePath) {
    std::unordered_map<char, int> freqMap;
    std::ifstream inFile(filePath, std::ios::binary);
    
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return freqMap;
    }

    char ch;
    while (inFile.get(ch)) {
        freqMap[ch]++;
    }

    inFile.close();
    return freqMap;
}

// Phase 1: Build the Huffman Tree using a Min-Heap
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freqMap) {
    // Creating a min-heap using our custom comparator from huffman.h
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> minHeap;

    // Step A: Create a leaf node for each character and push to heap
    for (const auto& pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Step B: Combine nodes until only the root remains
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        // Create a new parent node with the sum of frequencies
        HuffmanNode* parent = new HuffmanNode(left->freq + right->freq, left, right);
        
        minHeap.push(parent);
    }

    // The last remaining node is the root of our Huffman Tree
    return minHeap.top();
}