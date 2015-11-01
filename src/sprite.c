#include <SDL2/SDL.h>
#include <stdio.h>
#include "../../framework/sprite.h"
#include "sprite.h"
#include "../constants.h"
#include "../../system.h"
#include "../../component.h"

//void eventHitCallback(void*);
//void eventDamageCallback(void*);


void initComponentSprite() {
	addComponentToWorld(&COMPONENT_SPRITE, sizeof(SpriteComponent));
	createEvent(&EVENT_DRAW);

	startTextureManager();
	//createSystem(EVENT_HIT, COMPONENT_STATS, eventHitCallback);
	//createSystem(EVENT_DAMAGE, COMPONENT_STATS, eventDamageCallback);
}

void registerSprite(unsigned int entityId, char *spriteFilename) {
	addComponentToEntity(entityId, COMPONENT_SPRITE);
	SpriteComponent *sprite = getComponent(entityId, COMPONENT_SPRITE);

	sprite->texture = textureCreate(spriteFilename);
	//sprite->rect.w
}

void eventDrawCallback(void *data) {
	DrawEvent *drawEvent = data;
	SDL_Renderer *renderer = drawEvent->renderer;
	SpriteComponent *sprite = getComponent(drawEvent->entityId, COMPONENT_SPRITE);
	int cameraOffsetX = drawEvent->cameraOffsetX, cameraOffsetY = drawEvent->cameraOffsetY;
	//int viewportWidth = drawEvent->cameraViewportWidth, viewportHeight = drawEvent->cameraViewportHeight;
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
