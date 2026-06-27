#include "huffman.h"
#include <fstream>
#include <iostream>

// ==========================================
// PHASE 0: Taking Attendance (Frequency Map)
// ==========================================
// We need to know how many times each character appears in the file.
// Frequent characters will get shorter codes later to save space.
std::unordered_map<char, int> buildFrequencyMap(const std::string& filePath) {
    std::unordered_map<char, int> freqMap;
    
    // Open in binary mode so we don't accidentally skip weird hidden characters
    std::ifstream inFile(filePath, std::ios::binary);
    
    if (!inFile.is_open()) {
        std::cerr << "Error: Bro, I couldn't open the input file " << filePath << std::endl;
        return freqMap;
    }

    char ch;
    // Read the file byte by byte and keep a tally
    while (inFile.get(ch)) {
        freqMap[ch]++;
    }

    inFile.close();
    return freqMap;
}

// ==========================================
// PHASE 1: Building the Huffman Tree
// ==========================================
// We use a Min-Heap (priority queue) so that the rarest characters stay at the top of our queue.
// We combine the two rarest characters repeatedly until only one giant tree is left.
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> minHeap;

    // First, turn every character into a leaf node and throw it into the priority queue
    for (const auto& pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Keep combining the two smallest nodes until we only have the Root node left
    while (minHeap.size() > 1) {
        // Grab the two guys with the lowest frequencies
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        // Create a new parent node. Its frequency is the sum of both kids.
        // It doesn't hold a real character, so we pass '\0' (handled in constructor).
        HuffmanNode* parent = new HuffmanNode(left->freq + right->freq, left, right);
        
        // Throw the new parent back into the pile
        minHeap.push(parent);
    }

    // The last man standing is the root of our tree
    return minHeap.top();
}

// ==========================================
// PHASE 2: Generating the Encoding Map
// ==========================================
// We can't search the tree every time we want to compress a character (too slow).
// So, we walk the tree once, figure out the '0' and '1' path for every leaf, 
// and save it in a quick look-up dictionary (Hash Map).

void generateCodesHelper(HuffmanNode* root, std::string path, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root) return;

    // If both kids are null, we reached a leaf! Save the path we took to get here.
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = path;
        return;
    }

    // Walk left = add a '0' to the path
    generateCodesHelper(root->left, path + "0", huffmanCodes);
    
    // Walk right = add a '1' to the path
    generateCodesHelper(root->right, path + "1", huffmanCodes);
}

std::unordered_map<char, std::string> generateHuffmanCodes(HuffmanNode* root) {
    std::unordered_map<char, std::string> huffmanCodes;
    generateCodesHelper(root, "", huffmanCodes); // Start from the root with an empty path
    return huffmanCodes;
}

// ==========================================
// PHASE 3: The Magic Trick (Compress & Pack)
// ==========================================
// This is where we actually save space. We take our "1010" strings and pack them 
// into raw, hardcore memory bits using bitwise shifts.

void compressFile(const std::string& inputPath, const std::string& outputPath, std::unordered_map<char, std::string>& huffmanCodes) {
    std::ifstream inFile(inputPath, std::ios::binary);
    std::ofstream outFile(outputPath, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error: File open failed during compression." << std::endl;
        return;
    }

    char ch;
    unsigned char buffer = 0; // Think of this as an empty 8-slot egg carton
    int bitCount = 0;         // Tracks how many eggs (bits) we put in the carton

    // Read the original file again
    while (inFile.get(ch)) {
        std::string code = huffmanCodes[ch]; // Get our custom code (e.g., "110")

        // Pack each '1' and '0' into our 8-bit buffer
        for (char bit : code) {
            buffer = buffer << 1; // Slide everything left to make room at the end
            if (bit == '1') {
                buffer = buffer | 1; // If the string says '1', flip the last bit on
            }
            bitCount++;

            // Carton is full! We hit 8 bits.
            if (bitCount == 8) {
                outFile.write(reinterpret_cast<const char*>(&buffer), 1); // Flush it to the hard drive
                buffer = 0;   // Get a new empty carton
                bitCount = 0; // Reset the counter
            }
        }
    }

    // Edge Case: What if the file ended, but our last carton only has 3 bits in it?
    // We pad the rest with zeros so we can safely write the final byte to the file.
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount); 
        outFile.write(reinterpret_cast<const char*>(&buffer), 1);
    }

    inFile.close();
    outFile.close();
    std::cout << "-> File compressed beautifully!" << std::endl;
}

// ==========================================
// PHASE 4: Unpacking (Decompression)
// ==========================================
// Time to reverse the magic. We read the packed bits, walk the tree, 
// and spit out the original text characters.

void decompressFile(const std::string& inputPath, const std::string& outputPath, HuffmanNode* root, int totalChars) {
    std::ifstream inFile(inputPath, std::ios::binary);
    std::ofstream outFile(outputPath, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error: File open failed during decompression." << std::endl;
        return;
    }

    HuffmanNode* curr = root; // Start at the top of the tree
    char byte;
    int decodedChars = 0;     // Keeps track of how many real characters we've recovered

    // Read the compressed file 8 bits (1 byte) at a time
    while (inFile.get(byte) && decodedChars < totalChars) {
        
        // Look at each bit inside this byte, from left to right (7 down to 0)
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1; // Extract the exact bit we want to look at

            // Walk the tree: 0 means go left, 1 means go right
            if (bit == 0) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }

            // Did we hit a leaf? 
            if (!curr->left && !curr->right) {
                outFile.put(curr->ch); // Boom! Found the character. Write it to the new file.
                decodedChars++;
                curr = root;           // Teleport back to the top of the tree for the next character

                // If we recovered everything, break out early so we don't accidentally read the padding zeros
                if (decodedChars >= totalChars) {
                    break; 
                }
            }
        }
    }

    inFile.close();
    outFile.close();
    std::cout << "-> File decompressed successfully! Check the restored file." << std::endl;
}