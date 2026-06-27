#include <iostream>
#include "huffman.h"

// Recursive helper function to traverse the tree and print the binary codes
void printTreeStructure(HuffmanNode* root, std::string path = "") {
    if (!root) return;

    // If it's a leaf node (actual character), print its path
    if (!root->left && !root->right) {
        if (root->ch == '\n') std::cout << "\\n : " << path << std::endl;
        else if (root->ch == ' ') std::cout << "' ' : " << path << std::endl;
        else std::cout << root->ch << "   : " << path << std::endl;
        return;
    }

    // Traverse left (append '0') and right (append '1')
    printTreeStructure(root->left, path + "0");
    printTreeStructure(root->right, path + "1");
}

int main(int argc, char* argv[]) {
    // Ensure the user provided a file path
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    
    // Execute Phase 0
    std::unordered_map<char, int> freqMap = buildFrequencyMap(filePath);
    if (freqMap.empty()) {
        std::cout << "File is empty or could not be processed." << std::endl;
        return 1;
    }

    // Execute Phase 1
    std::cout << "\nBuilding Huffman Tree..." << std::endl;
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Verify the tree
    std::cout << "\n--- Generated Huffman Codes (Preview) ---" << std::endl;
    printTreeStructure(root);

    return 0;
}