#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

// #include <GL/gl.h>

uint64_t lastTick;
uint16_t deltaTime;

float tempFloat = 0.0345;

struct {
	int w = 1000;
	int h = 600;
	int32_t ofsetX;
	int32_t ofsetY;
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
	bool upArrow;
	bool leftArrow;
	bool downArrow;
	bool rightArrow;
} key;

bool running = 1;

void innit() {
	std::ofstream outputFileStream;
	outputFileStream.open("test.txt", std::ios::out|std::ios::binary);
	outputFileStream.write((char*) &tempFloat, sizeof(float));


	// std::ifstream inputFileStream;
	// inputFileStream.open("test.txt", std::ios::in|std::ios::binary);
    // inputFileStream.read((char*) &float, sizeof(float));
}

class decay {
	private:
		double pow255[1024] = {};
	public:
		void innit() {
			pow255[0] = 1;
			std::cout << pow255[0];
			for (int i = 1; i < 1024; ++i) {
				pow255[i] = pow255[i-1] * 127/128;
				// std::cout << pow255[i] << '\n';
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