#include <SDL2/SDL.h>
#include <stdbool.h>

unsigned int COMPONENT_SPRITE;
unsigned int EVENT_DRAW;

#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
	SDL_Texture *texture;
	//SDL_Point centerPoint, lastCenter;
	int x, lastX;
	int y, lastY;
	int width, lastWidth;
	int height, lastHeight;
	int angle, lastAngle;
	int alpha, lastAlpha;
	int layerIndex;
	double scaleW, lastScaleW;
	double scaleH, lastScaleH;
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

#endif

void initComponentSprite(void);
void destroyComponentSprite(void);
void registerSprite(unsigned int, char*);

