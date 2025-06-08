#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

uint64_t lastTick;
uint16_t deltaTime;

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
	double Vectx;
	double Vecty;
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

		// SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_NONE);

		// SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_HORIZONTAL);
		// SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_NONE);
		

		// SDL_Delay(1000/72);		
}