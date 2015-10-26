#include <SDL2/SDL.h>
#include <stdbool.h>

//typedef struct Sprite Sprite;

#ifndef SPRITE_H
#define SPRITE_H

typedef struct World World;

typedef struct {
	SDL_Rect rect;
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	void (*drawCallback)(struct World*, unsigned int);
	Uint8 alpha;
	int staticX, staticY;
	int r, g, b;
	int rMax, gMax, bMax;
	int rMin, gMin, bMin;
	int cameraLayer;
	double rReal, gReal, bReal;
	double rTintMod, gTintMod, bTintMod;
	double realAlpha, fadeRate, widthScaleRate, widthScale, heightScaleRate, heightScale, angle;
	double widthScaleMod, heightScaleMod, widthScaleMax, heightScaleMax;
	double fadeRateMod;
	bool obeyScrolling;
	bool alwaysDraw;
	bool dontScale;
	int z;
} Sprite;

#endif


Sprite* spriteCreate(char*);
Sprite* getSprite(World*, unsigned int);
SDL_Surface* surfaceCreate(char*);
SDL_Texture* textureCreate(char*);
SDL_Texture* textureCreateFromSurface(SDL_Surface*);
void shutdownSprites(void);
void spriteSetTexture(Sprite*, char*);
void spriteSetTextureEx(Sprite*, SDL_Texture*);
void surfaceDestroy(SDL_Surface*);
void textureDestroy(SDL_Texture*);
void renderSprite(Sprite*);
void renderRect(Sprite*);
void textureRender(SDL_Texture*, SDL_Renderer*, int, int);
void textureRenderWithScale(SDL_Texture*, SDL_Renderer*, int, int, double, double);
