#include "engine.hpp"
#include "../include/JSON/json.hpp"

#include <vector>

#define TILESIZE 36
#define SPEED 1.3

using namespace nlohmann;

int tempNum;
bool flag;
bool jump;

void game() {
    expDecay power{}; // initialises exponential decay to calculate player friction

    power.innit("src/data/number.bin");

    if (SDL_Init(SDL_INIT_EVERYTHING) == 1) {
		std::cout << SDL_GetError;
	}

    std::ifstream ifs("data/levels/blockData.json");
    json tileData = json::parse(ifs);

	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    SDL_Texture* render;

	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_TARGETTEXTURE);

    SDL_Surface* blocks;
    SDL_Surface* playerImage;

	blocks = IMG_Load("data/images/blocks.png");
	playerImage = IMG_Load("data/images/player.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, blocks);
	SDL_Texture* playerSprite = SDL_CreateTextureFromSurface(rend, playerImage);

    SDL_Rect temp{0, 0, TILESIZE, TILESIZE};

    SDL_Rect playerPos{0, 0, 16, 24};

    SDL_Rect clip{0, 0, 12, 12};

    sprite player;
    player.y = -600;
    player.x = -1100;

    file reader;
    tile tiles[reader.load("data/levels/lvl2/level.bin", 6)];
    reader.read(tiles);

    // for (int i = 0; i < reader.length; ++i) {
    //     std::cout << "[" << tiles[i].x << ", " << tiles[i].y << ", " << tiles[i].type << "]\n";
    // } // debuging in file reader

    while (running) {
        SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);
        SDL_RenderClear(rend);
        
        inputs();

        for (int i = 0; i < reader.length; ++i) {
            temp.x = tiles[i].x * TILESIZE - screen.ofsetX;
            temp.y = (1 - tiles[i].y) * TILESIZE - screen.ofsetY;
            // if (colidetect(temp, SDL_Rect{110, 110, screen.w, screen.h})) {
                clip = {tileData["tiles"][tiles[i].type]["pos"][0], tileData["tiles"][tiles[i].type]["pos"][1], 12, 12};
                SDL_RenderCopy(rend, texture, &clip, &temp);
            // }
        }

        playerPos.x = round((screen.w - playerPos.w) / 2 + player.x - screen.ofsetX);
        playerPos.y = round((screen.h - playerPos.h) / 2 + player.y - screen.ofsetY);
        SDL_RenderCopyEx(rend, playerSprite, NULL, &playerPos, 0, 0, (player.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

        
        player.VectX += ((key.d || key.rightArrow) - (key.a || key.leftArrow)) * (power.sqr(deltaTime) + 1) * deltaTime * SPEED; // player movement & integral to account for friction
        if (jump && (key.w || key.upArrow)) {
            player.VectY = -600;
        }                
        jump = 0;

        player.VectY += deltaTime * 1.75;

        if (key.r) {
            player.VectY = 0;
            player.VectX = 0;
            player.y = -600;
            player.x = -1100;
        }

        flag = 0;
        for (int i = 0; i < reader.length; ++i) {
            if (tileData["tiles"][tiles[i].type]["collisions"] == true) {
                temp.x = tiles[i].x * TILESIZE - screen.ofsetX;
                temp.y = (1 - tiles[i].y) * TILESIZE - screen.ofsetY;
                if (colidetect(SDL_Rect{playerPos.x + int(player.VectX * deltaTime / 1000), playerPos.y, playerPos.w, playerPos.h}, temp)) {
                    // if (abs(playerPos.x - temp.x) > abs(playerPos.x + player.VectX * deltaTime / 1000 - temp.x)) {
                        if (!colidetect(SDL_Rect{playerPos.x + int(player.VectX * deltaTime / 1000), playerPos.y + 2, playerPos.w, playerPos.h}, temp)) {
                            flag = 0;
                        }
                        else if (!colidetect(SDL_Rect{playerPos.x + int(player.VectX * deltaTime / 1000), playerPos.y - 2, playerPos.w, playerPos.h}, temp)) {
                            flag = 0;
                        }
                        else {
                            flag = 1;
                            break;
                        }
                    // }
                }
            }
        }
        if (!flag) {
            player.x += player.VectX * deltaTime / 1000;
        }
        else {
            player.VectX = 0;
        }        

        playerPos.x = round((screen.w - playerPos.w) / 2 + player.x - screen.ofsetX);

        flag = 0;
        for (int i = 0; i < reader.length; ++i) {
            if (tileData["tiles"][tiles[i].type]["collisions"]) {
                temp.x = tiles[i].x * TILESIZE - screen.ofsetX;
                temp.y = (1 - tiles[i].y) * TILESIZE - screen.ofsetY;
                if (colidetect(SDL_Rect{playerPos.x, playerPos.y + int(player.VectY * deltaTime / 1000), playerPos.w, playerPos.h}, temp)) {
                    // if (abs(playerPos.y - temp.y) > abs(playerPos.y + player.VectY * deltaTime / 1000 - temp.y)) {
                        if (!colidetect(SDL_Rect{playerPos.x + 1, playerPos.y + int(player.VectY * deltaTime / 1000), playerPos.w, playerPos.h}, temp)) {
                            flag = 0;
                        }
                        else if (!colidetect(SDL_Rect{playerPos.x - 1, playerPos.y + int(player.VectY * deltaTime / 1000), playerPos.w, playerPos.h}, temp)) {
                            flag = 0;
                        }
                        else {
                            flag = 1;
                            if (player.VectY >= 0) {
                                jump = 1;
                            }
                            break;
                        }
                    // }
                }
            }
        }
        if (!flag) {
            player.y += player.VectY * deltaTime / 1000;
        }
        else {
            player.VectY = 0;
        }

        playerPos.y = round((screen.h - playerPos.h) / 2 + player.y - screen.ofsetY);

        for (int j = 0; j < reader.length; ++j) {
            temp.x = tiles[j].x * TILESIZE - screen.ofsetX;
            temp.y = (1 - tiles[j].y) * TILESIZE - screen.ofsetY;
            if (tiles[j].type == 5) {
                if (colidetect(playerPos, temp)) {
                    player.VectY = 0;
                    player.VectX = 0;
                    player.y = -600;
                    player.x = -1100;
                }
            }
        }


        // for (int i = 0; i < reader.length; ++i) {
        //     if (tileData["tiles"][tiles[i].type]["collisions"] && colidetect(playerPos, temp)) {
        //         temp.x = tiles[i].x * TILESIZE - screen.ofsetX;
        //         temp.y = (1 - tiles[i].y) * TILESIZE - screen.ofsetY;
        //         for (int j = 0; j < 16; ++j) {
        //             if (!colidetect(SDL_Rect{playerPos.x + j, playerPos.y, playerPos.h, playerPos.w}, temp)) {
        //                 player.x += j;
        //                 break;
        //             }
        //             if (!colidetect(SDL_Rect{playerPos.x - j, playerPos.y, playerPos.h, playerPos.w}, temp)) {
        //                 player.x -= j;
        //                 break;
        //             }
        //             if (!colidetect(SDL_Rect{playerPos.x, playerPos.y + j, playerPos.h, playerPos.w}, temp)) {
        //                 player.y += j;
        //                 break;
        //             }
        //             if (!colidetect(SDL_Rect{playerPos.x, playerPos.y - j, playerPos.h, playerPos.w}, temp)) {
        //                 player.y -= j;
        //                 break;
        //             }
        //         }
        //     }
        // }

        playerPos.x = round((screen.w - playerPos.w) / 2 + player.x - screen.ofsetX);
        playerPos.y = round((screen.h - playerPos.h) / 2 + player.y - screen.ofsetY);
    
        screen.ofsetX -= (screen.ofsetX - player.x) * (1 - power.sqr(deltaTime));
        screen.ofsetY -= (screen.ofsetY - player.y) * (1 - power.sqr(deltaTime));

        player.VectX *= power.sqr(deltaTime);
        player.VectY *= power.sqr(int(deltaTime / 6));

        if (player.VectX > 0) {
            player.flip = 0;
        }
        else if (player.VectX < 0) {
            player.flip = 1;
        }
        
        SDL_RenderPresent(rend);

        // SDL_Delay(1);
    }
}