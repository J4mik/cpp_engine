#include "src\gameloop.hpp"

int level = 1;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO);
	TTF_Init();
	MIX_Init();

	SDL_Window* win = SDL_CreateWindow("Flashblade", screen.w, screen.h, SDL_WINDOW_RESIZABLE);
	SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_Renderer* rend = SDL_CreateRenderer(win, NULL);
	SDL_SetRenderDrawColor(rend, 2, 4, 7, 255);
	
	load();

    while (game(level, win, rend)) {
        ++level;
    }

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	TTF_Quit();
	MIX_Quit();
    return 0;
}