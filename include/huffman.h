#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <string>
#include <queue>
#include <vector>

// The building block of our Huffman Tree
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    // Constructor for leaf nodes (actual characters)
    HuffmanNode(char character, int frequency) 
        : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

    // Constructor for internal nodes (combined nodes)
    HuffmanNode(int frequency, HuffmanNode* l, HuffmanNode* r) 
        : ch('\0'), freq(frequency), left(l), right(r) {}
};

// Custom comparator for Priority Queue (Min-Heap)
struct CompareNodes {
    bool operator()(HuffmanNode* const& n1, HuffmanNode* const& n2) {
        return n1->freq > n2->freq;
    }
};

// Function declarations for all Phases
std::unordered_map<char, int> buildFrequencyMap(const std::string& filePath);
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freqMap);
std::unordered_map<char, std::string> generateHuffmanCodes(HuffmanNode* root);
void compressFile(const std::string& inputPath, const std::string& outputPath, std::unordered_map<char, std::string>& huffmanCodes);
void decompressFile(const std::string& inputPath, const std::string& outputPath, HuffmanNode* root, int totalChars);

#endif // HUFFMAN_H