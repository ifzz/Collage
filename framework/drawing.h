#include <SDL2/SDL.h>

#define MAX_FONT_NAME_LEN 32

void drawPixel(Uint32*, int, int, int, Uint32);
void startFonts(void);
void shutdownFonts(void);
SDL_Texture* textureGenerateTile(SDL_Renderer*, SDL_Texture*, int, int);
SDL_Texture* renderFont(SDL_Renderer*, char*, int, int, int, int, int*, int*, char*);
void drawRotatedBox(SDL_Renderer*, int, int[][2], int);
