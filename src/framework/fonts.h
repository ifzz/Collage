#include <SDL2/SDL.h>


void startFonts(void);
void shutdownFonts(void);
SDL_Texture *renderFont(SDL_Renderer*, char*, int, int, int, int, int*, int*,
		char*);
