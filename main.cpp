#include "src\gameloop.hpp"

int level = 1;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", screen.w, screen.h, SDL_WINDOW_RESIZABLE);
	SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_Renderer* rend = SDL_CreateRenderer(win, NULL);

	ByteBounce = TTF_OpenFont("data/fonts/ByteBounce.ttf", 96);
	fColor.r = 255;
    fColor.g = 0;
    fColor.b = 127;
	fontSurface = TTF_RenderText_Solid(ByteBounce, "hello", 10, fColor);
	SDL_BlitSurface(fontSurface, NULL, &screene, &fontPos);

	MIX_Init();

    while (game(level, win, rend)) {
        ++level;
    }
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}