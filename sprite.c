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
void eventSetSpriteJig(unsigned int, void*);

void initComponentSprite() {
	addComponentToWorld(&COMPONENT_SPRITE, sizeof(SpriteComponent));
	createEvent(&EVENT_DRAW);
	createEvent(&EVENT_SET_ALPHA);
	createEvent(&EVENT_SET_SPRITE_JIG);

	startTextureManager();
	createSystem(EVENT_TIMESTEP_START, COMPONENT_SPRITE, eventSpriteTimestepHandler);
	createSystem(EVENT_DRAW, COMPONENT_SPRITE, eventDrawCallback);
	createSystem(EVENT_SET_POSITION, COMPONENT_SPRITE,
			eventSetSpritePositionCallback);
	createSystem(EVENT_SET_ALPHA, COMPONENT_SPRITE,
			eventSetSpriteAlphaCallback);
	createSystem(EVENT_SET_SPRITE_JIG, COMPONENT_SPRITE,
			eventSetSpriteJig);
}

void destroyComponentSprite() {
	destroyTextureManager();
}

void registerSprite(unsigned int entityId, char *spriteFilename) {
	//#TODO: Go back to using rects!
	addComponentToEntity(entityId, COMPONENT_SPRITE);
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	sprite->texture = textureCreate(spriteFilename);
	
	SDL_QueryTexture(sprite->texture, NULL, NULL,
			&sprite->width, &sprite->height);

	sprite->lastWidth = sprite->width;
	sprite->lastHeight = sprite->height;
	sprite->lastX = -333;
	sprite->lastY = -333;
	sprite->x = 0; 
	sprite->y = 0; 
	sprite->lastJigX = 0;
	sprite->lastJigY = 0;
	sprite->jigX = 0;
	sprite->jigX = 0;
	sprite->jigX = 0;
	sprite->jigY = 0;
	sprite->lastScaleW = 1.;
	sprite->lastScaleH = 1.;
	sprite->scaleW = 1.;
	sprite->scaleH = 1.;
	sprite->lastAlpha = 255;
	sprite->alpha = 255;
}

void eventSpriteTimestepHandler(unsigned int entityId, void *data) {
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	sprite->lastJigX = sprite->jigX;
	sprite->lastJigY = sprite->jigY;
	sprite->lastX = sprite->x;
	sprite->lastY = sprite->y;
	sprite->lastScaleW = sprite->scaleW;
	sprite->lastScaleH = sprite->scaleH;
	sprite->lastAlpha = sprite->alpha;
}

void eventDrawCallback(unsigned int entityId, void *data) {
	DrawEvent *drawEvent = data;
	double delta = drawEvent->delta;
	SDL_Renderer *renderer = drawEvent->renderer;
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);
	int cameraOffsetX = drawEvent->cameraOffsetX, cameraOffsetY = drawEvent->cameraOffsetY;
	double cameraZoom = drawEvent->cameraZoom;
	SDL_Rect renderRect;

	double tempScaleW = interp(sprite->lastScaleW, sprite->scaleW, delta);
	double tempScaleH = interp(sprite->lastScaleH, sprite->scaleH, delta);

	int tempJigX = interp(sprite->lastJigX, sprite->jigX, delta);
	int tempJigY = interp(sprite->lastJigY, sprite->jigY, delta);

	renderRect.w = sprite->width * tempScaleW;
	renderRect.h = sprite->height * tempScaleH;

	int tempAlpha = interp(sprite->lastAlpha, sprite->alpha, delta);

	SDL_SetTextureAlphaMod(sprite->texture, tempAlpha);

	renderRect.x = interp(sprite->lastX, sprite->x, delta) + tempJigX - cameraOffsetX;
	renderRect.y = interp(sprite->lastY, sprite->y, delta) + tempJigY - cameraOffsetY;

	renderRect.x += round((sprite->width - renderRect.w) / 2.f);
	renderRect.y += round((sprite->height - renderRect.h) / 2.f);

	SDL_RenderCopyEx(renderer, sprite->texture, NULL, &renderRect, sprite->angle, NULL, SDL_FLIP_NONE);
}

void eventSetSpritePositionCallback(unsigned int entityId, void *data) {
	SetPositionEvent *newPosition = (SetPositionEvent*)data;
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	/*if (sprite->lastX == -333 && sprite->lastY == -333) {*/
		/*sprite->lastX = newPosition->x;*/
		/*sprite->lastY = newPosition->y;*/

		/*printf("Must be new...\n");*/
	/*}*/

	sprite->x = newPosition->x;
	sprite->y = newPosition->y;
}

void eventSetSpriteJig(unsigned int entityId, void *data) {
	SetSpriteJigEvent *pos = (SetSpriteJigEvent*)data;
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	sprite->jigX = pos->x;
	sprite->jigY = pos->y;
}

void eventSetSpriteAlphaCallback(unsigned int entityId, void *data) {
	SetAlphaEvent *newAlpha = (SetAlphaEvent*)data;
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	/*sprite->lastAlpha = sprite->alpha;*/
	sprite->alpha = newAlpha->alpha;
}
