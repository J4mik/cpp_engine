#include <iostream>
#include <fstream>

int16_t tile;

bool counter = 0;

namespace file {
    
    void read(std::basic_string<char> path) {
        
        std::ifstream inputFileStream;
        inputFileStream.open(path, std::ios::in|std::ios::binary);
        inputFileStream.seekg(0, std::ifstream::end);
        counter = int(inputFileStream.tellg()) / 3;
        int16_t tiles[counter] = {};
        inputFileStream.seekg(0, std::ifstream::beg);
        for (int i = counter; i > 0, --i) {
            for (int j = 0; j < 3; ++j) {
                counter = inputFileStream.tellg();
                try {
                    inputFileStream.read((char*) &tile, 2); // if doesn't work try: sizeof(uint16_t) instead of 2
                    if (counter < inputFileStream.tellg()) {
                        tiles[i][j] = tile;
                    }
                    else {
                        std::cout << "file length is not a factor of 48";
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