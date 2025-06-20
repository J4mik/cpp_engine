#include "engine.hpp"

#include <cmath>

void game() {
    decay power{}; // initialises exponential ot calculate player friction

    power.innit();

    if (SDL_Init(SDL_INIT_EVERYTHING) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_RESIZABLE);

	void SDL_SetWindowResizable(SDL_Window * window, SDL_bool resizable);

	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* blocks;
    SDL_Surface* playerImage;

	blocks = IMG_Load(".\\data\\images\\blocks.png");
	playerImage = IMG_Load(".\\data\\images\\player.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, blocks);
	SDL_Texture* playerSprite = SDL_CreateTextureFromSurface(rend, playerImage);

    SDL_Rect temp;
	temp.x = 0;
	temp.y = 0;
	temp.h = 48;
	temp.w = 48;

    SDL_Rect playerPos{0, 0, 24, 36};

    SDL_Rect rock{0, 0, 12, 12};
    SDL_Rect grass{0, 12, 12, 12};
	SDL_Rect path{0, 24, 12, 12};
    SDL_Rect dirt{0, 36, 12, 12};
	SDL_Rect sand{12, 0, 12, 12};

    sprite player;

    while (running) {
        SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);
        SDL_RenderClear(rend);
        inputs();
        temp.x = 0;
        SDL_RenderCopy(rend, texture, &dirt, &temp);
        temp.x = 48;
        SDL_RenderCopy(rend, texture, &grass, &temp);
        temp.x = 96;
        SDL_RenderCopy(rend, texture, &path, &temp);
        temp.x = 144;
        SDL_RenderCopy(rend, texture, &sand, &temp);
        temp.x = 192;
        SDL_RenderCopy(rend, texture, &rock, &temp);

        playerPos.x = round(player.x);
        playerPos.y = round(player.y);
        SDL_RenderCopyEx(rend, playerSprite, NULL, &playerPos, 0, 0, (player.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

        player.VectX += ((key.d || key.rightArrow) - (key.a || key.leftArrow)) * (power.sqr(deltaTime) + 1) * deltaTime; // player movement & integral to account for friction
        player.VectY += ((key.s || key.downArrow) - (key.w || key.upArrow)) * (power.sqr(deltaTime) + 1) * deltaTime;

        player.x += player.VectX * (power.sqr(deltaTime) + 1) * deltaTime / 1000; // integral to ensure same speed at all framerates
        player.y += player.VectY * (power.sqr(deltaTime) + 1) * deltaTime / 1000;

        player.VectX *= power.sqr(deltaTime);
        player.VectY *= power.sqr(deltaTime);


        if (player.VectX > 0) {
            player.flip = 0;
        }
        else if (player.VectX < 0) {
            player.flip = 1;
        }
        
        SDL_RenderPresent(rend);
        // std::cout << deltaTime << "\n";
        SDL_Delay(1000/180);
    }
}