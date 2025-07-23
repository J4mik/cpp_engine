#include <iostream>
#include <fstream>

#include <cmath>

// int16_t tiles[0][2];

class file {
    public:
    int16_t tiles[841][2];
    int16_t length;
    void read(std::basic_string<char> path) {
        std::ifstream inputFileStream;
        inputFileStream.open(path, std::ios::in|std::ios::binary);
        inputFileStream.seekg(0, std::fstream::end);
        length = math.floor(inputFileStream.tellg() / 6); // the 6 is for the size of "struct" [int16_t, int16_t, int16_t] this can be replaced if a different struct is used
        int16_t tiles[length][2];
        --length;
        inputFileStream.seekg(0, std::fstream::beg);
        for (int i = length; i >= 0; --i) {
            for (int j = 0; j < 3; ++j) {
                try {
                    inputFileStream.read((char*) &tiles[i][j], 2); // if doesn't work try: sizeof(uint16_t) instead of 2
                }
                catch (std::ifstream::failure& error) {
                    inputFileStream.close();
                    return;
                }
            }
        }
        inputFileStream.close();
    }
};