#include "huffman.h"
#include <fstream>
#include <iostream>

// --- PHASE 0: Frequency Map ---
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

// --- PHASE 1: Build Huffman Tree ---
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> minHeap;

    for (const auto& pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        HuffmanNode* parent = new HuffmanNode(left->freq + right->freq, left, right);
        minHeap.push(parent);
    }

    return minHeap.top();
}

// --- PHASE 2: Generate Encoding Map ---
// Recursive Helper Function (DFS logic)
void generateCodesHelper(HuffmanNode* root, std::string path, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root) return;

    // Agar leaf node hai (no left, no right), toh map me save kar lo
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = path;
        return;
    }

    // Traverse left (add '0') and right (add '1')
    generateCodesHelper(root->left, path + "0", huffmanCodes);
    generateCodesHelper(root->right, path + "1", huffmanCodes);
}

// Ye function bahar se call hoga aur pura map banakar return karega
std::unordered_map<char, std::string> generateHuffmanCodes(HuffmanNode* root) {
    std::unordered_map<char, std::string> huffmanCodes;
    generateCodesHelper(root, "", huffmanCodes);
    return huffmanCodes;
}