#include "engine.hpp"
#include "../include/JSON/json.hpp"

#include <iostream>

#include <vector>

using namespace nlohmann;

#define TILESIZE 36
#define TILESIZEINPIXELS 12
#define SPEED 1.7
#define GRAVITY 1.8
#define FALLTIME 40

struct {
    float x;
    float y;
} spawn;

void reset(sprite (&players)) {
    players.x = spawn.x;
    players.y = spawn.y;
    players.VectX = 0;
    players.VectY = 0;
}

bool game(int lvl, SDL_Window* win, SDL_Renderer* rend) {

    int SCALE = (TILESIZE / TILESIZEINPIXELS);

    int tempNum;
    bool flag;
    uint8_t jump;

    struct {
        float x;
        float y;
    } end;

    expDecay power{}; // initialises exponential decay to calculate player friction

    power.innit("src/data/number.bin");

    std::ifstream ifs("data/levels/blockData.json");
    json tileData = json::parse(ifs);
    SDL_Texture* texture = IMG_LoadTexture(rend, "data/images/blocks.png");
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	SDL_Texture* playerSprite = IMG_LoadTexture(rend, "data/images/player.png");
    SDL_SetTextureScaleMode(playerSprite, SDL_SCALEMODE_NEAREST);

    SDL_FRect temp{0, 0, TILESIZE, TILESIZE};

    SDL_FRect playerPos{0, 0, 16, 24};

    SDL_FRect clip{0, 0, 12, 12};

    file reader;
    tile tiles[reader.load(std::string(std::string("data/levels/lvl")+std::string(std::to_string(lvl))+std::string("/level.bin")), 6)];
    if (reader.error){
        return 0;
    }
    reader.read(tiles);
    reader.close();

    for (int j = 0; j < reader.length; ++j) {
        if (tileData["tiles"][tiles[j].type]["damage"] == true) {
            ++tempNum;
        }
        if (tileData["tiles"][tiles[j].type]["portal"] == true) {
            end.x = tiles[j].x * TILESIZE;
            end.y = (1 - tiles[j].y) * TILESIZE;
        }
    }

    tile spikes[tempNum];
    tempNum = 0;

    for (int j = 0; j < reader.length; ++j) {
        if (tileData["tiles"][tiles[j].type]["damage"] == true) {
            spikes[tempNum] = tiles[j];
            ++tempNum;
        }
    }

    std::ifstream config(std::string("data/levels/lvl")+std::string(std::to_string(lvl))+std::string("/levelConfig.json"));
    json configFile = json::parse(config);

    spawn.x = configFile["level"]["spawn"][0];
    spawn.y = configFile["level"]["spawn"][1];

    sprite player;
    reset(player);
    inputs();
    while (running) {
        SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);
        SDL_RenderClear(rend);
        
        inputs();

        for (int i = 0; i < reader.length; ++i) {
            temp.x = tiles[i].x * TILESIZE - screen.ofsetX + screen.w / 2;
            temp.y = (1 - tiles[i].y) * TILESIZE - screen.ofsetY + screen.h / 2;
            clip = {tileData["tiles"][tiles[i].type]["pos"][0], tileData["tiles"][tiles[i].type]["pos"][1], TILESIZEINPIXELS, TILESIZEINPIXELS};
            SDL_RenderTexture(rend, texture, &clip, &temp);
        }

        playerPos.x = (screen.w) / 2 + player.x - screen.ofsetX;
        playerPos.y = (screen.h) / 2 + player.y - screen.ofsetY;
        SDL_RenderTextureRotated(rend, playerSprite, NULL, &playerPos, 0, 0, (player.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

        
        player.VectX += ((key.d || key.rightArrow) - (key.a || key.leftArrow)) * (power.sqr(deltaTime) + 1) * deltaTime * SPEED; // player movement & integral to account for friction
        if (jump && (key.w || key.upArrow)) {
            player.VectY = -620;
            jump = 0;
        }    
        if (deltaTime < jump) {        
            jump -= deltaTime;
        }
        else {
            jump = 0;
        }

        player.VectY += deltaTime * GRAVITY;

        if (key.r) {
           reset(player);
        }

        // player collisions
        flag = 0;
        for (int i = 0; i < reader.length; ++i) {
            if (tileData["tiles"][tiles[i].type]["collisions"] == true) {
                temp.x = tiles[i].x * TILESIZE;
                temp.y = (1 - tiles[i].y) * TILESIZE;
                if (colidetect(SDL_FRect{player.x + player.VectX * deltaTime / 1000, player.y, playerPos.w, playerPos.h}, temp)) {
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
                if (colidetect(SDL_FRect{player.x, player.y + player.VectY * deltaTime / 1000, playerPos.w, playerPos.h}, temp)) {
                    flag = 1;
                    if (player.VectY >= 0) {
                        jump = FALLTIME;
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

        for (int j = 0; j < tempNum; ++j) {
            if (tileData["tiles"][spikes[j].type]["damage"] == true) {
                if (colidetect(SDL_FRect{player.x, player.y, playerPos.w, playerPos.h}, 
                            SDL_FRect{spikes[j].x * TILESIZE + float(tileData["tiles"][spikes[j].type]["hitbox"][0]) * SCALE, 
                            (1 - spikes[j].y) * TILESIZE + float(tileData["tiles"][spikes[j].type]["hitbox"][1]) * SCALE, 
                            float(tileData["tiles"][spikes[j].type]["hitbox"][2]) * SCALE, float(tileData["tiles"][spikes[j].type]["hitbox"][3]) * SCALE})) {
                    reset(player);
                }
            }
        }

        if (colidetect(SDL_FRect{player.x, player.y, playerPos.w, playerPos.h},
        SDL_FRect{end.x, end.y, TILESIZE, TILESIZE})) {
            return(1);
        }
    
        // screen ofset
        screen.ofsetX -= (screen.ofsetX - player.x + player.w / 2) * (1 - power.sqr(deltaTime));
        screen.ofsetY -= (screen.ofsetY - player.y + player.h / 2) * (1 - power.sqr(deltaTime));

        player.VectX *= power.sqr(deltaTime);
        player.VectY *= power.sqr(int(deltaTime / 8));

        if (player.VectX > 0) {
            player.flip = 0;
        }
        else if (player.VectX < 0) {
            player.flip = 1;
        }
        SDL_RenderPresent(rend);
        // SDL_UpdateWindowSurface(win);
        SDL_Delay(5);
    }
    return(0);
}