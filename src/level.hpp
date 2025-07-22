#include <iostream>
#include <fstream>

int16_t tile;
int16_t tiles[] = {};

bool counter = 0;

namespace file {
    
    void read(std::basic_string<char> path) {
        
       std::ifstream inputFileStream;
        inputFileStream.open(path, std::ios::in|std::ios::binary);
        while (true) {
            for (int i = 0; i < 3; ++i) {
                counter = inputFileStream.tellg();
                try {
                    inputFileStream.read((char*) &tile, 2); // if doesn't work try: sizeof(uint16_t) instead of 2
                    if (counter < inputFileStream.tellg()) {
                        tiles[i] = tile;
                    }
                    else {
                        std::cout << tiles[2];
                        return;
                    }
                }
                catch (std::ifstream::failure& error) {
                    inputFileStream.close();
                }
            }
        }
    }
}