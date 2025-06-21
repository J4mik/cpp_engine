//
// Created by jenskromdijk on 14/03/25.
//

#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "./constants.hpp"

class App {
public:
    App() = default;

    ~App() {
        if (!_closed) {
            close();
        }
    }

    int start() {
        if (!init()) {
            std::cerr << "GAME::ERROR Failed to initialize!" << std::endl;
            return -1;
        }
        if (!loadMedia()) {
            std::cerr << "GAME::ERROR Failed to load media!" << std::endl;
            return -1;
        }
        run();
        close();
        return 0;
    }

    bool init() {
        std::cout << "GAME::INITIALIZING";

        // ------------- INITIALIZE SDL & Subsystems ------------- //
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "..!\n";
            std::cout << "GAME::INIT::ERROR SDL_Init: " << SDL_GetError() << std::endl;
            return false;
        }
        std::cout << ".";

        // initialize SDL_Image
        int imgFlags {IMG_INIT_PNG};
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cout << ".!\n";
            std::cout << "GAME::INIT::ERROR IMG_Init: " << IMG_GetError() << std::endl;
            return false;
        }
        std::cout << ".";

        // initialize SDL_TTF
        if (TTF_Init() == -1) {
            std::cout << ".!\n";
            std::cout << "GAME::INIT::ERROR TTF_Init: " << TTF_GetError() << std::endl;
            return false;
        }
        std::cout << ".";

        if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        {
            std::cout << "..!\n";
            std::cout << "GAME::INIT::ERROR Mix_OpenAudio: " << Mix_GetError() << std::endl;
            return false;
        }
        std::cout << ".";

        // --------------- INITIALIZE SDL components -------------- //

        _window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (_window == nullptr) {
            std::cout << "..!\n";
            std::cout << "GAME::INIT::ERROR Failed to create SDL_Window! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        std::cout << ".";

        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (_renderer == nullptr) {
            std::cout << "..!\n";
            std::cout << "GAME::INIT::ERROR Failed to create SDL_Renderer! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

        std::cout << ".\n";
        std::cout << "GAME::INIT::OK!" << std::endl;
        return true;
    }

    // load game assets
    bool loadMedia() {
        std::cout << "GAME::LOADING...\n";
        std::cout << "GAME::LOADING::OK!" << std::endl;
        return true;
    }

    // destroy renderer & window and quit SDL subsystems
    void close() {
        if (!_closed) {
            std::cout << "GAME::CLOSING...\n";
            SDL_DestroyRenderer(_renderer);
            SDL_DestroyWindow(_window);
            TTF_Quit();
            IMG_Quit();
            Mix_Quit();
            SDL_Quit();
            std::cout << "GAME::CLOSED!\n";
            _closed = true;
        }
    }

    void run() {
        SDL_Event event;
        bool running {true};

        do {
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_MOUSEMOTION)
                {
                    SDL_GetGlobalMouseState(&_mouseX, &_mouseY);
                    _mouseX -= _windowX;
                    _mouseY -= _windowY;
                } else if (event.type == SDL_WINDOWEVENT)
                {
                    // handle window events
                    _width = event.window.data1 / 3;
                    _height = event.window.data2 / 3;
                    SDL_RenderPresent(_renderer);
                }
            }

            SDL_GetWindowPosition(_window, &_windowX, &_windowY);

            // clear screen
            SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(_renderer);

            SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);

            SDL_RenderPresent(_renderer);

        } while (running);
    }

    [[nodiscard]] int getWidth() const {
        return _width;
    }

    [[nodiscard]] int getHeight() const {
        return _height;
    }

    [[nodiscard]] int getMouseX() const {
        return _mouseX;
    }

    [[nodiscard]] int getMouseY() const {
        return _mouseY;
    }

    [[nodiscard]] int getWindowX() const {
        return _windowX;
    }

    [[nodiscard]] int getWindowY() const {
        return _windowY;
    }

private:
    SDL_Window* _window {nullptr};
    SDL_Renderer* _renderer {nullptr};

    bool _closed{false};

    int _width {WINDOW_WIDTH};
    int _height {WINDOW_HEIGHT};
    int _mouseX {0};
    int _mouseY {0};
    int _windowX {0};
    int _windowY {0};
};

#endif //APP_H
