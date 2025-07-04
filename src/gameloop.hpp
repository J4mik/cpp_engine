#include "engine.hpp"

#include <cmath>
#include <vector>

const std::vector<GLfloat> vertex{
    0.8f, -0.8f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.8f, -0.8f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.8f, 0.0f,  0.0f, 0.0f, 1.0f
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

void game() {
    decay power{}; // initialises exponential ot calculate player friction

    power.innit();

    if (SDL_Init(SDL_INIT_EVERYTHING) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    SDL_Texture* render;

	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    SDL_SetRenderTarget(rend, render);

    SDL_GLContext shader = nullptr;

    SDL_Surface* blocks;
    SDL_Surface* playerImage;

	blocks = IMG_Load(".\\data\\images\\blocks.png");
	playerImage = IMG_Load(".\\data\\images\\player.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, blocks);
	SDL_Texture* playerSprite = SDL_CreateTextureFromSurface(rend, playerImage);

    SDL_Rect temp;

    const SDL_Rect screen2{0, 0, screen.w, screen.h};

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); 

    shader = SDL_GL_CreateContext(win);

    // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);

    while (running) {
        SDL_GetWindowSizeInPixels(win, &screen.w, &screen.h);
        SDL_RenderClear(rend);

        // SDL_GL_SwapWindow(win);
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

        // SDL_RenderClear(renderer);

        // SDL_RenderCopy(renderer, render, NULL, &screen2);

        // SDL_RenderPresent(renderer);

        // std::cout << deltaTime << "\n";
        SDL_Delay(1000/180);
    }
}