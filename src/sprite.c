#include <SDL2/SDL.h>
#include <stdio.h>
#include "framework/sprite.h"
#include "framework/numbers.h"
#include "sprite.h"
#include "worldPosition.h"
#include "system.h"
#include "component.h"

void eventDrawCallback(unsigned int, void*);
void eventSetSpritePositionCallback(unsigned int, void*);

void initComponentSprite() {
	addComponentToWorld(&COMPONENT_SPRITE, sizeof(SpriteComponent));
	createEvent(&EVENT_DRAW);

	startTextureManager();
	createSystem(EVENT_DRAW, COMPONENT_SPRITE, eventDrawCallback);
	createSystem(EVENT_SET_POSITION, COMPONENT_SPRITE, eventSetSpritePositionCallback);
}

void destroyComponentSprite() {
	destroyTextureManager();
}

void registerSprite(unsigned int entityId, char *spriteFilename) {
	//#TODO: Go back to using rects!
	addComponentToEntity(entityId, COMPONENT_SPRITE);
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;

	printf("Sprite size = %lu\n", sizeof(sprite));
	sprite->texture = textureCreate(spriteFilename);
	
	SDL_QueryTexture(sprite->texture, NULL, NULL, &sprite->width, &sprite->height);
	sprite->x = 0; 
	sprite->y = 0; 
	//sprite->rect.x = 30;
	//sprite->rect.y = 30;
	sprite->scaleW = 1.;
	sprite->scaleH = 1.;
	/*//sprite->centerPoint.x = sprite->rect.w / 2;*/
	/*//sprite->centerPoint.y = sprite->rect.h / 2;*/
	sprite->alpha = 255;
}

void eventDrawCallback(unsigned int entityId, void *data) {
	DrawEvent *drawEvent = data;
	double delta = drawEvent->delta;
	SDL_Renderer *renderer = drawEvent->renderer;
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;
	int cameraOffsetX = drawEvent->cameraOffsetX, cameraOffsetY = drawEvent->cameraOffsetY;
	double cameraZoom = drawEvent->cameraZoom;
	SDL_Rect renderRect;

	printf("Delta: %f\n", drawEvent->delta);

	renderRect.w = interp(sprite->width * sprite->scaleW, sprite->lastWidth * sprite->lastScaleW, delta);
	renderRect.h = interp(sprite->height * sprite->scaleH, sprite->lastHeight * sprite->lastScaleH, delta);

	SDL_SetTextureAlphaMod(sprite->texture, interp(sprite->alpha, sprite->lastAlpha, delta));

	renderRect.x = interp(sprite->x, sprite->lastX, delta) - cameraOffsetX;
	renderRect.y = interp(sprite->y, sprite->lastY, delta) - cameraOffsetY;

	renderRect.x -= (renderRect.w - interp(sprite->width, sprite->lastWidth, delta) / 2);
	renderRect.y -= (renderRect.h - interp(sprite->height, sprite->lastHeight, delta) / 2);

	printf("%i, %i\n", renderRect.x, renderRect.y);

	//#TODO: Center point
	SDL_RenderCopyEx(renderer, sprite->texture, NULL, &renderRect, sprite->angle, NULL, SDL_FLIP_NONE);
}

void oldEventDrawCallback(unsigned int entityId, void *data) {
	DrawEvent *drawEvent = data;
	SDL_Renderer *renderer = drawEvent->renderer;
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;
	int cameraOffsetX = drawEvent->cameraOffsetX, cameraOffsetY = drawEvent->cameraOffsetY;
	double cameraZoom = drawEvent->cameraZoom;
	SDL_Rect renderRect;

	printf("Delta: %f\n", drawEvent->delta);

	//renderRect.w = sprite->rect.w * sprite->scaleW;
	//renderRect.h = sprite->rect.h * sprite->scaleH;

	//SDL_SetTextureAlphaMod(sprite->texture, sprite->alpha);

	//renderRect.x = sprite->rect.x - cameraOffsetX;
	//renderRect.y = sprite->rect.y - cameraOffsetY;

	//renderRect.x -= (renderRect.w - sprite->rect.w) / 2;
	//renderRect.y -= (renderRect.h - sprite->rect.h) / 2;

	//SDL_RenderCopyEx(renderer, sprite->texture, NULL, &renderRect, sprite->angle, &sprite->centerPoint, SDL_FLIP_NONE);
}

void eventSetSpritePositionCallback(unsigned int entityId, void *data) {
	SetPositionEvent *newPosition = (SetPositionEvent*)data;
	SpriteComponent *sprite = &getComponent(entityId, COMPONENT_SPRITE)->sprite;

	sprite->x = newPosition->x;
	sprite->y = newPosition->y;

	printf("Set SPRITE_POS: %i, %i\n", sprite->x, sprite->y);
}
