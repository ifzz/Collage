#include <SDL2/SDL.h>
#include <stdbool.h>

unsigned int COMPONENT_SPRITE;
unsigned int EVENT_DRAW;
typedef struct SpriteComponent SpriteComponent;

#ifndef SPRITE_H
#define SPRITE_H


struct SpriteComponent {
	SDL_Texture *texture;
	SDL_Rect rect, lastRect;
	SDL_Point centerPoint, lastCenter;
	int angle, lastAngle;
	int alpha, lastAlpha;
	int layerIndex;
	double scaleW, lastScaleW;
	double scaleH, lastScaleH;
};

typedef struct {
	SDL_Renderer *renderer;
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

