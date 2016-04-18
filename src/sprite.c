#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include "framework/sprite.h"
#include "framework/numbers.h"
#include "sprite.h"
#include "worldPosition.h"
#include "system.h"
#include "component.h"
#include "timestep.h"

void eventSpriteTimestepHandler(unsigned int, void*);
void eventDrawCallback(unsigned int, void*);
void eventSetSpritePositionCallback(unsigned int, void*);
void eventSetSpriteAlphaCallback(unsigned int, void*);

void initComponentSprite() {
	addComponentToWorld(&COMPONENT_SPRITE, sizeof(SpriteComponent));
	createEvent(&EVENT_DRAW);
	createEvent(&EVENT_SET_ALPHA);

	startTextureManager();
	createSystem(EVENT_TIMESTEP, COMPONENT_SPRITE, eventSpriteTimestepHandler);
	createSystem(EVENT_DRAW, COMPONENT_SPRITE, eventDrawCallback);
	createSystem(EVENT_SET_POSITION, COMPONENT_SPRITE, eventSetSpritePositionCallback);
	createSystem(EVENT_SET_ALPHA, COMPONENT_SPRITE, eventSetSpriteAlphaCallback);
}

void destroyComponentSprite() {
	destroyTextureManager();
}

void registerSprite(unsigned int entityId, char *spriteFilename) {
	//#TODO: Go back to using rects!
	addComponentToEntity(entityId, COMPONENT_SPRITE);
	SpriteComponent *sprite = &(getComponent(entityId, COMPONENT_SPRITE)->sprite);

	sprite->texture = textureCreate(spriteFilename);
	
	SDL_QueryTexture(sprite->texture, NULL, NULL, &sprite->width, &sprite->height);

	sprite->lastWidth = sprite->width;
	sprite->lastHeight = sprite->height;
	sprite->x = 0; 
	sprite->y = 0; 
	sprite->lastScaleW = 1.;
	sprite->lastScaleH = 1.;
	sprite->scaleW = 1.;
	sprite->scaleH = 1.;
	sprite->lastAlpha = 255;
	sprite->alpha = 255;
}

void eventSpriteTimestepHandler(unsigned int entityId, void *data) {
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;

	sprite->lastX = sprite->x;
	sprite->lastY = sprite->y;
}

void eventDrawCallback(unsigned int entityId, void *data) {
	DrawEvent *drawEvent = data;
	double delta = drawEvent->delta;
	SDL_Renderer *renderer = drawEvent->renderer;
	SpriteComponent *sprite = &(getComponent(entityId, COMPONENT_SPRITE)->sprite);
	int cameraOffsetX = drawEvent->cameraOffsetX, cameraOffsetY = drawEvent->cameraOffsetY;
	double cameraZoom = drawEvent->cameraZoom;
	SDL_Rect renderRect;

	renderRect.w = interp(sprite->lastWidth * sprite->lastScaleW, sprite->width * sprite->scaleW, delta);
	renderRect.h = interp(sprite->lastHeight * sprite->lastScaleH, sprite->height * sprite->scaleH, delta);

	/*sprite->alpha = interp(sprite->alpha, sprite->lastAlpha, delta);*/

	SDL_SetTextureAlphaMod(sprite->texture, sprite->alpha);

	renderRect.x = interp(sprite->lastX, sprite->x, delta) - cameraOffsetX;
	renderRect.y = interp(sprite->lastY, sprite->y, delta) - cameraOffsetY;

	renderRect.x -= (renderRect.w - interp(sprite->lastWidth, sprite->width, delta) / 2);
	renderRect.y -= (renderRect.h - interp(sprite->lastHeight, sprite->height, delta) / 2);

	//#TODO: Center point
	SDL_RenderCopyEx(renderer, sprite->texture, NULL, &renderRect, sprite->angle, NULL, SDL_FLIP_NONE);
}

void eventSetSpritePositionCallback(unsigned int entityId, void *data) {
	SetPositionEvent *newPosition = (SetPositionEvent*)data;
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;

	/*sprite->lastX = sprite->x;*/
	/*sprite->lastY = sprite->y;*/
	sprite->x = newPosition->x;
	sprite->y = newPosition->y;
}

void eventSetSpriteAlphaCallback(unsigned int entityId, void *data) {
	SetAlphaEvent *newAlpha = (SetAlphaEvent*)data;
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;

	/*sprite->lastAlpha = sprite->alpha;*/
	sprite->alpha = newAlpha->alpha;
}
