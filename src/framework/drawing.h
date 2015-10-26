#include <SDL2/SDL.h>


void drawPixel(Uint32*, int, int, int, Uint32);
void startFonts(void);
void shutdownFonts(void);
SDL_Texture *renderFont(SDL_Renderer*, char*, int, int, int, int, int*, int*, char*);
