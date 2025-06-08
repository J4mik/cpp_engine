#include "src\gameloop.hpp"

int main(int argc, char *argv[]) {
    gameloop();
    // SDL_DestroyTexture(tex);
	// SDL_DestroyRenderer(rend);
	// SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}