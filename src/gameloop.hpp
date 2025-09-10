#include "engine.hpp"
#include "../include/JSON/json.hpp"

#include <iostream>

#include <vector>

#define TILESIZE 36
#define TILESIZEINPIXELS 12
#define SPEED 1.3

int SCALE = (TILESIZE / TILESIZEINPIXELS);


using namespace nlohmann;

int tempNum;
bool flag;
bool jump;

int lvl = 1;

struct {
    int x;
    int y;
} spawn;

void reset(sprite (&players)) {
    players.x = spawn.x;
    players.y = spawn.y;
    players.VectX = 0;
    players.VectY = 0;
}

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

    file reader;
    tile tiles[reader.load(std::string(std::string("data/levels/lvl")+std::string("1")+std::string("/level.bin")), 6)];
    reader.read(tiles);

    std::ifstream config(std::string("data/levels/lvl")+std::string("1")+std::string("/levelConfig.json"));
    json configFile = json::parse(config);

    spawn.x = configFile["level"]["spawn"][0];
    spawn.y = configFile["level"]["spawn"][1];

    sprite player;
    reset(player);

    while (running) {
        SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);
        SDL_RenderClear(rend);
        
        inputs();

        for (int i = 0; i < reader.length; ++i) {
            temp.x = tiles[i].x * TILESIZE - screen.ofsetX + screen.w / 2;
            temp.y = (1 - tiles[i].y) * TILESIZE - screen.ofsetY + screen.h / 2;
            // if (colidetect(temp, SDL_Rect{110, 110, screen.w, screen.h})) {
                clip = {tileData["tiles"][tiles[i].type]["pos"][0], tileData["tiles"][tiles[i].type]["pos"][1], TILESIZEINPIXELS, TILESIZEINPIXELS};
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
           reset(player);
        }

        flag = 0;
        for (int i = 0; i < reader.length; ++i) {
            if (tileData["tiles"][tiles[i].type]["collisions"] == true) {
                temp.x = tiles[i].x * TILESIZE;
                temp.y = (1 - tiles[i].y) * TILESIZE;
                if (colidetect(SDL_Rect{int((-playerPos.w) / 2 + player.x + player.VectX * deltaTime / 1000), int((-playerPos.h) / 2 + player.y), playerPos.w, playerPos.h}, temp)) {
                    flag = 1;
                    break;
                }
            }
        }
        if (!flag) {
            player.x += player.VectX * deltaTime / 1000;
        }
        else {
            player.VectX = 0;
        }        

        flag = 0;
        for (int i = 0; i < reader.length; ++i) {
            if (tileData["tiles"][tiles[i].type]["collisions"]) {
                temp.x = tiles[i].x * TILESIZE;
                temp.y = (1 - tiles[i].y) * TILESIZE;
                if (colidetect(SDL_Rect{int((-playerPos.w) / 2 + player.x), int((-playerPos.h) / 2 + player.y + player.VectY * deltaTime / 1000), playerPos.w, playerPos.h}, temp)) {
                    flag = 1;
                    if (player.VectY >= 0) {
                        jump = 1;
                    }
                    break;
                }
            }
        }
        if (!flag) {
            player.y += player.VectY * deltaTime / 1000;
        }
        else {
            player.VectY = 0;
        }

        for (int j = 0; j < reader.length; ++j) {
            if (tileData["tiles"][tiles[j].type]["damage"] == true) {
                if (colidetect(SDL_Rect{round((-playerPos.w) / 2 + player.x), round((-playerPos.h) / 2 + player.y), playerPos.w, playerPos.h}, 
                            SDL_Rect{tiles[j].x * TILESIZE + int(tileData["tiles"][tiles[j].type]["hitbox"][0]) * SCALE, 
                            (1 - tiles[j].y) * TILESIZE + int(tileData["tiles"][tiles[j].type]["hitbox"][1]) * SCALE, 
                            int(tileData["tiles"][tiles[j].type]["hitbox"][2]) * SCALE, int(tileData["tiles"][tiles[j].type]["hitbox"][3]) * SCALE})) {
                    reset(player);
                }
            }
        }

    
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