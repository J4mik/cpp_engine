#include "src\gameloop.hpp"

int level = 1;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    // innit("test.bin");
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_TARGETTEXTURE);

    while (game(level, win, rend)) {
        ++level;
    }
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}