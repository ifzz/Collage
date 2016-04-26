#include <SDL2/SDL.h>
#include <stdbool.h>

unsigned int COMPONENT_SPRITE;
unsigned int EVENT_DRAW;
unsigned int EVENT_SET_ALPHA;
unsigned int EVENT_SET_SPRITE_JIG;

#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
	SDL_Texture *texture;
	int x, lastX;
	int y, lastY;
	int lastJigX, lastJigY;
	int jigX, jigY;
	int width, lastWidth;
	int height, lastHeight;
	short int angle, lastAngle;
	short int alpha, lastAlpha;
	//int layerIndex;
	float scaleW, lastScaleW;
	float scaleH, lastScaleH;
} SpriteComponent;

typedef struct {
	SDL_Renderer *renderer;
	int cameraOffsetX;
	int cameraOffsetY;
	//int cameraViewportWidth;
	//int cameraViewportHeight;
	double delta;
	double cameraZoom;
} DrawEvent;

typedef struct {
	int alpha;
} SetAlphaEvent;

typedef struct {
	int x, y;
} SetSpriteJigEvent;

#endif

void initComponentSprite(void);
void destroyComponentSprite(void);
void registerSprite(unsigned int, char*);

