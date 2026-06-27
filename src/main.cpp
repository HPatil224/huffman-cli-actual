#include <iostream>
#include "huffman.h"

int main(int argc, char* argv[]) {
    // Ab hume 3 files chahiye (Input, Compressed Output, Restored Output)
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <input.txt> <compressed.huf> <restored.txt>" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    std::string compressedPath = argv[2];
    std::string restoredPath = argv[3];
    
    // Phase 0
    std::cout << "\n[1] Reading file..." << std::endl;
    std::unordered_map<char, int> freqMap = buildFrequencyMap(inputPath);
    if (freqMap.empty()) return 1;

    // Calculate total characters for decompression logic
    int totalChars = 0;
    for (const auto& pair : freqMap) {
        totalChars += pair.second;
    }

    // Phase 1
    std::cout << "[2] Building Huffman Tree..." << std::endl;
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Phase 2
    std::cout << "[3] Generating Encoding Map..." << std::endl;
    std::unordered_map<char, std::string> huffmanCodes = generateHuffmanCodes(root);

    // Phase 3
    std::cout << "[4] Compressing file and packing bits..." << std::endl;
    compressFile(inputPath, compressedPath, huffmanCodes);

    // Phase 4
    std::cout << "[5] Decompressing file back to original..." << std::endl;
    decompressFile(compressedPath, restoredPath, root, totalChars);

    std::cout << "\n--- ALL PHASES COMPLETE! ---" << std::endl;
    return 0;
}