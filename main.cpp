#include "src\gameloop.hpp"

int level = 1;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", screen.w, screen.h, SDL_WINDOW_RESIZABLE);
	SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_Renderer* rend = SDL_CreateRenderer(win, NULL);

    while (game(level, win, rend)) {
        ++level;
    }
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}