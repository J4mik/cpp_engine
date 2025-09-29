#include "src\gameloop.hpp"

int level = 1;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) == 1) {
		std::cout << SDL_GetError;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", screen.w, screen.h, SDL_WINDOW_RESIZABLE);
	SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_Renderer* rend = SDL_CreateRenderer(win, NULL);

	MIX_Init();
	SDL_AudioSpec audioSpec;
    audioSpec.format = SDL_AUDIO_F32;
    audioSpec.channels = 2;
    audioSpec.freq = 44100;
    MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);
    auto music = MIX_LoadAudio(mixer, "data/audio/level.wav", 1);
    MIX_PlayAudio(mixer, music);

    while (game(level, win, rend)) {
        ++level;
    }
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}