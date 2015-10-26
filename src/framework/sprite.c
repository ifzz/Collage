#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "list.h"
#include "display.h"
#include "sprite.h"
#include "../camera.h"

linkedList_t *TEXTURE_MANAGER = NULL;

typedef struct textureManagerEntry {
	char *name;
	SDL_Texture *texture;
} textureManagerEntry;


Sprite* spriteCreate(char *filename) {
	Sprite *newSprite = malloc(sizeof(Sprite));

	newSprite->texture = textureCreate(filename);
	newSprite->renderer = displayGetRenderer();
	newSprite->rect.x = 0;
	newSprite->rect.y = 0;
	newSprite->rect.w = 10;
	newSprite->rect.h = 10;
	newSprite->obeyScrolling = false;

	SDL_QueryTexture(newSprite->texture, NULL, NULL, &newSprite->rect.w, &newSprite->rect.h);

	return newSprite;
}

Sprite* getSprite(World *world, unsigned int entityId) {
	return &world->sprite[entityId];
}

void spriteSetTexture(Sprite *sprite, char *filename) {
	sprite->texture = textureCreate(filename);
	sprite->renderer = displayGetRenderer();
	//sprite->rect.x = 10;
	//sprite->rect.y = 10;

	SDL_QueryTexture(sprite->texture, NULL, NULL, &sprite->rect.w, &sprite->rect.h);
}

void spriteSetTextureEx(Sprite *sprite, SDL_Texture *texture) {
	sprite->texture = texture;
	sprite->renderer = displayGetRenderer();
	//sprite->rect.x = 10;
	//sprite->rect.y = 10;

	SDL_QueryTexture(sprite->texture, NULL, NULL, &sprite->rect.w, &sprite->rect.h);
}

void renderSprite(Sprite *sprite) {
	SDL_Renderer *renderer = displayGetRenderer();
	SDL_Rect renderRect;

	renderRect.w = sprite->rect.w * sprite->widthScale;
	renderRect.h = sprite->rect.h * sprite->heightScale;

	SDL_SetTextureAlphaMod(sprite->texture, sprite->alpha);
	SDL_SetTextureColorMod(sprite->texture, sprite->r, sprite->g, sprite->b);
	//if (sprite->scaleRate) {
	//	SDL_RenderSetScale(sprite->renderer, sprite->scale, sprite->scale);
	//}

	if (sprite->obeyScrolling) {
		renderRect.x = sprite->staticX;
		renderRect.y = sprite->staticY;

		//renderRect.x -= (getViewportWidth() / 4) * (getCameraZoom() - 1);
		//renderRect.y -= (getViewportHeight() / 4) * (getCameraZoom() - 1);

		renderRect.x -= (renderRect.w - sprite->rect.w) / 2;
		renderRect.y -= (renderRect.h - sprite->rect.h) / 2;

		SDL_RenderCopy(sprite->renderer, sprite->texture, NULL, &renderRect);
	} else {
		renderRect.x = sprite->rect.x - getCameraOffsetX();
		renderRect.y = sprite->rect.y - getCameraOffsetY();
		
		if (sprite->cameraLayer > 0) {
			int cameraXCenter = getCameraOffsetX() + ((getViewportWidth() / getCameraZoom()) / 2);
			int layerXCenter = renderRect.x + ((getViewportWidth() / getCameraZoom()) / 2);
			int xDiff = cameraXCenter - layerXCenter;
			
			renderRect.x += xDiff * (sprite->cameraLayer * .03);
		}

		//renderRect.x -= (getViewportWidth() / 4) * (getCameraZoom() - 1);
		//renderRect.y -= (getViewportHeight() / 4) * (getCameraZoom() - 1);

		renderRect.x -= (renderRect.w - sprite->rect.w) / 2;
		renderRect.y -= (renderRect.h - sprite->rect.h) / 2;

		SDL_RenderCopyEx(sprite->renderer, sprite->texture, NULL, &renderRect, sprite->angle, NULL, SDL_FLIP_NONE);
	}
}

void renderRect(Sprite *sprite) {
	SDL_Rect renderRect;
	renderRect.w = sprite->rect.w;
	renderRect.h = sprite->rect.h;

	if (sprite->obeyScrolling) {
		renderRect.x = sprite->staticX;
		renderRect.y = sprite->staticY;

		//renderRect.x -= (getViewportWidth() / 4) * (getCameraZoom() - 1);
		//renderRect.y -= (getViewportHeight() / 4) * (getCameraZoom() - 1);

		SDL_SetRenderDrawColor(sprite->renderer, sprite->r, sprite->g, sprite->b, sprite->alpha);
		SDL_RenderFillRect(sprite->renderer, &renderRect);
	} else {
		renderRect.x = sprite->rect.x - getCameraOffsetX();
		renderRect.y = sprite->rect.y - getCameraOffsetY();

		renderRect.x -= (renderRect.w - sprite->rect.w) / 2;
		renderRect.y -= (renderRect.h - sprite->rect.h) / 2;
		
		if (sprite->cameraLayer > 0) {
			int cameraXCenter = getCameraOffsetX() + ((getViewportWidth() / getCameraZoom()) / 2);
			int layerXCenter = renderRect.x + ((getViewportWidth() / getCameraZoom()) / 2);
			int xDiff = cameraXCenter - layerXCenter;
			
			renderRect.x += xDiff * (sprite->cameraLayer * .03);
		}

		SDL_SetRenderDrawColor(sprite->renderer, sprite->r, sprite->g, sprite->b, sprite->alpha);
		SDL_RenderFillRect(sprite->renderer, &renderRect);
	}
}

SDL_Surface* surfaceCreate(char *filename) {
	printf("**FATAL** THIS IS BROKEN\n");

	SDL_Surface *surface = NULL;
	SDL_Surface *tempSurface = IMG_Load(filename);
	SDL_Surface *windowSurface = displayGetWindowSurface();

	if (tempSurface == NULL) {
		printf("Problem loading sprite: %s\n", SDL_GetError());

		displayDestroy();
	} else {
		surface = SDL_ConvertSurface(tempSurface, windowSurface->format, 0);
		surfaceDestroy(tempSurface);
	}

	return surface;
}

void surfaceDestroy(SDL_Surface *surface) {
	SDL_FreeSurface(surface);
}

void deleteTextureFromList(void *data) {
	textureManagerEntry *texture = data;

	free(texture->name);
	SDL_DestroyTexture(texture->texture);
}

void shutdownSprites() {
	deleteLinkedList(TEXTURE_MANAGER);
}

void addTexture(char *filename, SDL_Texture *texture) {
	if (!TEXTURE_MANAGER)
		TEXTURE_MANAGER = createLinkedList(&deleteTextureFromList);

	textureManagerEntry *textureEntry = malloc(sizeof(textureManagerEntry));

	textureEntry->name = malloc(strlen(filename) + 1);
	strcpy(textureEntry->name, filename);
	textureEntry->texture = texture;

	addListItem(TEXTURE_MANAGER, textureEntry);

	printf("TextureMan: %s\n", textureEntry->name);
}

SDL_Texture* getTextureWithName(char *filename) {
	if (!TEXTURE_MANAGER)
		TEXTURE_MANAGER = createLinkedList(&deleteTextureFromList);

	listItem_t *textureList = TEXTURE_MANAGER->head;

	if (textureList == NULL)
		return NULL;

	while (textureList) {
		textureManagerEntry *item = textureList->item;

		if (!strcmp(item->name, filename)) {
			return item->texture;
		}

		textureList = textureList->next;
	}

	return NULL;
}

SDL_Texture* textureCreate(char *filename) {
	SDL_Renderer *renderer = displayGetRenderer();
	SDL_Texture *texture = getTextureWithName(filename);

	if (texture == NULL) {
		texture = IMG_LoadTexture(renderer, filename);

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		addTexture(filename, texture);
	}

	if (texture == NULL){
		printf("Problem loading texture: %s\n", SDL_GetError());

		displayDestroy();
	}

	return texture;
}

SDL_Texture* textureCreateFromSurface(SDL_Surface *surface) {
	SDL_Renderer *renderer = displayGetRenderer();
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == NULL){
		printf("Problem loading texture: %s\n", SDL_GetError());

		displayDestroy();
	}

	return texture;
}

void textureDestroy(SDL_Texture *texture) {
	SDL_DestroyTexture(texture);
}

void textureRender(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void textureRenderWithScale(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, double wMod, double hMod) {
	SDL_Rect dst;
	SDL_Point pnt;
	dst.x = x;
	dst.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	
	pnt.x = dst.w / 2;
	pnt.y = dst.h / 2;
	dst.w *= wMod;
	dst.h *= hMod;
	
	//SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_RenderCopyEx(renderer, texture, NULL, &dst, 0, &pnt, SDL_FLIP_NONE);
}
