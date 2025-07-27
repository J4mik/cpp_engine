#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

bool colidetect(SDL_Rect a, SDL_Rect b) {
    if (b.x <= a.x <= b.x + b.w || b.x <= a.x + a.w <= b.x + b.w) {
        if (b.y <= a.y <= b.y + b.h || b.y <= a.y + a.h <= b.y + b.h) {
            return 1;
        }
    }
    return 0;
}