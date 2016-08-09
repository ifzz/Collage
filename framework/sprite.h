#include <SDL2/SDL.h>
#include <stdbool.h>



void startTextureManager(void);
void destroyTextureManager(void);
//Sprite* spriteCreate(char*);
//Sprite* getSprite(World*, unsigned int);
SDL_Surface* surfaceCreate(char*);
SDL_Texture* textureCreate(char*);
SDL_Texture* textureCreateFromSurface(SDL_Surface*);
void shutdownSprites(void);
//void spriteSetTexture(Sprite*, char*);
//void spriteSetTextureEx(Sprite*, SDL_Texture*);
void surfaceDestroy(SDL_Surface*);
void textureDestroy(SDL_Texture*);
//void renderSprite(Sprite*);
//void renderRect(Sprite*);
void textureRender(SDL_Renderer*, SDL_Texture*, int, int);
void textureRenderWithScale(SDL_Renderer*, SDL_Texture*, int, int, double, double);
void textureRenderEx(SDL_Renderer*, SDL_Texture*, int, int, int, double, double);
