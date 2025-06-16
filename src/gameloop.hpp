#include "engine.hpp"

#include "./include/JSON/json.hpp"

void game() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_RESIZABLE);

	void SDL_SetWindowResizable(SDL_Window * window, SDL_bool resizable);

	SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);

	std::cout << screen.h;

	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* blocks;
    SDL_Surface* player;

	blocks = IMG_Load(".\\data\\images\\blocks.png");
	player = IMG_Load(".\\data\\images\\player.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, blocks);
	SDL_Texture* playerSprite = SDL_CreateTextureFromSurface(rend, player);

    SDL_Rect temp;
	temp.x = 0;
	temp.y = 0;
	temp.h = 48;
	temp.w = 48;

    SDL_Rect dirt{0, 0, 16, 16};
    SDL_Rect grass{0, 16, 16, 16};
	SDL_Rect path{0, 32, 16, 16};
	SDL_Rect sand{0, 48, 16, 16};

    while (running) {
        SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);
        inputs();
        temp.x = 0;
        SDL_RenderCopy(rend, texture, &dirt, &temp);
        temp.x = 48;
        SDL_RenderCopy(rend, texture, &grass, &temp);
        temp.x = 96;
        SDL_RenderCopy(rend, texture, &path, &temp);
        temp.x = 144;
        SDL_RenderCopy(rend, texture, &sand, &temp);

        temp.x = 300;
        SDL_RenderCopy(rend, playerSprite, NULL, &temp);

        
        SDL_RenderPresent(rend);
        std::cout << deltaTime << "\n";
        SDL_Delay(1000/30);
    }
}