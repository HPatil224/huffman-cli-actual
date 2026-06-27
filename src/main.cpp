#include <iostream>
#include "huffman.h"

int main(int argc, char* argv[]) {
    // Check if the file path is provided in terminal
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    
    // Phase 0: Get Character Frequencies
    std::unordered_map<char, int> freqMap = buildFrequencyMap(filePath);
    if (freqMap.empty()) {
        std::cout << "File is empty or could not be processed." << std::endl;
        return 1;
    }

    // Phase 1: Build the Binary Tree
    std::cout << "Building Huffman Tree..." << std::endl;
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Phase 2: Generate the Encoding Table Map
    std::cout << "Generating Encoding Map...\n" << std::endl;
    std::unordered_map<char, std::string> huffmanCodes = generateHuffmanCodes(root);

    // Verify Phase 2: Print the map contents
    std::cout << "--- Stored Encoding Table ---" << std::endl;
    for (const auto& pair : huffmanCodes) {
        if (pair.first == '\n') std::cout << "\\n : " << pair.second << std::endl;
        else if (pair.first == ' ') std::cout << "' ' : " << pair.second << std::endl;
        else std::cout << pair.first << "   : " << pair.second << std::endl;
    }

    return 0;
}