#include "level.hpp"
#include "collisions.hpp"

uint64_t lastTick;
uint16_t deltaTime;

struct {
	int w = 1000;
	int h = 600;
	double ofsetX;
	double ofsetY;
} screen;

class sprite {
	public:
	double x;
	double y;
	uint16_t h;
	uint16_t w;
	double VectX;
	double VectY;
	bool flip;
};

struct {
	bool w;
	bool a;
	bool s;
	bool d;
	bool r;
	bool upArrow;
	bool leftArrow;
	bool downArrow;
	bool rightArrow;
} key;

bool running = 1;

// void innit(std::basic_string<char> file) {
// 	std::ofstream outputFileStream;
// 	outputFileStream.open(file, std::ios::out|std::ios::binary);
// 	outputFileStream.write((char*) &tempFloat, 4); // size if uint32_t is 4
// 	outputFileStream.close();


// 	std::ifstream inputFileStream;
// 	inputFileStream.open(file, std::ios::in|std::ios::binary);
//     inputFileStream.read((char*) &newFloat, 4); // if doesn't work try: sizeof(uint32_t) instead of 4
// 	inputFileStream.close();
// 	std::cout << newFloat;
// }

class chunk {
	public:
		int16_t chunk[0][3];
};

constexpr int length = 1024;
// std::ios_base::failure error;
class expDecay {
	private:
		double pow255[length];
	public:
		void innit(std::basic_string<char> path) {

			std::ifstream FileStream;
			FileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try {
				FileStream.open(path, std::ios::in|std::ios::binary);
				for (int i = 0; i < length; ++i) {
					FileStream.read((char*) &pow255[i], 8);
				}
				FileStream.close();
			}
			catch (std::ifstream::failure& error){
				std::cout << "Error";
				if (!pow255[0] == 1) {
					std::cout << " reading from file '" << path << "', recreating file contents\n";
					std::ofstream FileStream;
					FileStream.open(path, std::ios::out|std::ios::binary);
					pow255[0] = 1;
					FileStream.write((char*) &pow255[0], 8);
					for (int i = 1; i < length; ++i) {
						pow255[i] = pow255[i-1] * 127/128;
						FileStream.write((char*) &pow255[i], 8);
					}
				}
			}
		}
		double sqr(int number) {
			if (number < 1024) {
				return(pow255[number]);
			}
			return(0);
		}
};

void inputs() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
            switch (event.type) {
			case SDL_QUIT: 
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_w:
						key.w = 1;
						break;
					case SDLK_a:
						key.a = 1;
						break;
					case SDLK_s:
						key.s = 1;
						break;
					case SDLK_d:
						key.d = 1;
						break;

					case SDLK_r:
						key.r = 1;
						break;

					case SDLK_UP:
						key.upArrow = 1;
						break;
					case SDLK_LEFT:
						key.leftArrow = 1;
						break;
					case SDLK_DOWN:
						key.downArrow = 1;
						break;
					case SDLK_RIGHT:
						key.rightArrow = 1;
						break;
				}
			break;
			case SDL_KEYUP: 
				switch (event.key.keysym.sym) {
					case SDLK_w:
						key.w = 0;
						break;
					case SDLK_a:
						key.a = 0;
						break;
					case SDLK_s:
						key.s = 0;
						break;
					case SDLK_d:
						key.d = 0;
						break;

					case SDLK_r:
						key.r = 0;
						break;

					case SDLK_UP:
						key.upArrow = 0;
						break;
					case SDLK_LEFT:
						key.leftArrow = 0;
						break;
					case SDLK_DOWN:
						key.downArrow = 0;
						break;
					case SDLK_RIGHT:
						key.rightArrow = 0;
						break;
				}
			}
        }
    
	deltaTime = SDL_GetTicks64() - lastTick;
	lastTick = SDL_GetTicks64();
}