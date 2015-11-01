#include <SDL2/SDL.h>
#include <stdbool.h>

unsigned int COMPONENT_SPRITE;
unsigned int EVENT_DRAW;

#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
	SDL_Texture *texture;
	SDL_Rect rect;
	SDL_Point centerPoint;
	int angle;
	int alpha;
	int layerIndex;
	double scaleW;
	double scaleH;
} SpriteComponent;

typedef struct {
	SDL_Renderer *renderer;
	unsigned int entityId;
	int cameraOffsetX;
	int cameraOffsetY;
	//int cameraViewportWidth;
	//int cameraViewportHeight;
	double cameraZoom;
} DrawEvent;

#endif

void initComponentSprite(void);
void destroyComponentSprite(void);
void registerSprite(unsigned int, char*);

