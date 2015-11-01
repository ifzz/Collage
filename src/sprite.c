#include <SDL2/SDL.h>
#include <stdio.h>
#include "framework/sprite.h"
#include "sprite.h"
#include "worldPosition.h"
#include "system.h"
#include "component.h"

void eventDrawCallback(unsigned int, void*);
void eventSetPositionCallback(unsigned int, void*);

void initComponentSprite() {
	addComponentToWorld(&COMPONENT_SPRITE, sizeof(SpriteComponent));
	createEvent(&EVENT_DRAW);

	startTextureManager();
	createSystem(EVENT_DRAW, COMPONENT_SPRITE, eventDrawCallback);
	createSystem(EVENT_SET_POSITION, COMPONENT_SPRITE, eventSetPositionCallback);
}

void destroyComponentSprite() {
	destroyTextureManager();
}

void registerSprite(unsigned int entityId, char *spriteFilename) {
	addComponentToEntity(entityId, COMPONENT_SPRITE);
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	sprite->texture = textureCreate(spriteFilename);
	
	SDL_QueryTexture(sprite->texture, NULL, NULL, &sprite->rect.w, &sprite->rect.h);
	sprite->rect.x = 30;
	sprite->rect.y = 30;
	sprite->scaleW = 1.;
	sprite->scaleH = 1.;
	sprite->centerPoint.x = sprite->rect.w / 2;
	sprite->centerPoint.y = sprite->rect.h / 2;
	sprite->alpha = 255;
}

void eventDrawCallback(unsigned int entityId, void *data) {
	DrawEvent *drawEvent = data;
	SDL_Renderer *renderer = drawEvent->renderer;
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);
	int cameraOffsetX = drawEvent->cameraOffsetX, cameraOffsetY = drawEvent->cameraOffsetY;
	double cameraZoom = drawEvent->cameraZoom;
	SDL_Rect renderRect;

	renderRect.w = sprite->rect.w * sprite->scaleW;
	renderRect.h = sprite->rect.h * sprite->scaleH;

	SDL_SetTextureAlphaMod(sprite->texture, sprite->alpha);

	renderRect.x = sprite->rect.x - cameraOffsetX;
	renderRect.y = sprite->rect.y - cameraOffsetY;

	renderRect.x -= (renderRect.w - sprite->rect.w) / 2;
	renderRect.y -= (renderRect.h - sprite->rect.h) / 2;

	SDL_RenderCopyEx(renderer, sprite->texture, NULL, &renderRect, sprite->angle, &sprite->centerPoint, SDL_FLIP_NONE);
}

void eventSetPositionCallback(unsigned int entityId, void *data) {
	SetPositionEvent *newPosition = (SetPositionEvent*)data;
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	sprite->rect.x = newPosition->x;
	sprite->rect.y = newPosition->y;

	printf("Set sprite pos: %i, %i\n", sprite->rect.x, sprite->rect.y);
}
