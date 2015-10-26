#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "list.h"


typedef struct fontManagerEntry {
	char *name;
	TTF_Font *font;
	int size;
} fontManagerEntry;

linkedList_t *FONT_MANAGER = NULL;


void deleteFontFromList(void *data) {
	fontManagerEntry *fME = data;
	
	//free(fME->filename);
	TTF_CloseFont(fME->font);
}

void startFonts() {
	FONT_MANAGER = createLinkedList(&deleteFontFromList);
}

void shutdownFonts() {
	listItem_t *fontList = FONT_MANAGER->head;
	
	while (fontList) {
		fontManagerEntry *item = fontList->item;
		
		TTF_CloseFont(item->font);
		
		fontList = fontList->next;
	}
}

TTF_Font *getFontWithNameAndSize(char *name, int size) {
	listItem_t *fontList = FONT_MANAGER->head;
	
	while (fontList) {
		fontManagerEntry *item = fontList->item;
		
		if (item->size == size && !strcmp(item->name, name))
			return item->font;
		
		fontList = fontList->next;
	}
	
	//printf("Could not find font: %s\n", name);
	
	return NULL;
}

TTF_Font *addFontWithNameAndSize(char *filename, char *name, int size) {
	fontManagerEntry *fontEntry = malloc(sizeof(fontManagerEntry));
	
	fontEntry->name = malloc(strlen(name) + 1);
	
	//strcpy(soundEntry->filename, filename);
	strcpy(fontEntry->name, name);
	
	fontEntry->font = TTF_OpenFont(filename, size);
	fontEntry->size = size;
	
	addListItem(FONT_MANAGER, fontEntry);
	
	printf("Added new font: %s, size=%i\n", name, size);
	
	return fontEntry->font;
}

void drawPixel(Uint32 *pixels, int x, int y, int w, Uint32 pixel){
    pixels[(y * w) + x] = pixel;
}

SDL_Texture *renderFont(SDL_Renderer *renderer, char *fontName, int r, int g, int b, int size, int *width, int *height, char *text) {
	TTF_Font *font = getFontWithNameAndSize(fontName, size);
	
	if (font == NULL)
		font = addFontWithNameAndSize(fontName, fontName, size);

	SDL_Color color = {r, g, b, 255};
	int _w, _h;

	SDL_Surface *surfaceMessage = TTF_RenderText_Blended(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_QueryTexture(texture, NULL, NULL, &_w, &_h);
	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	//printf("TT: %i, %i\n", _w, _h);
	
	/*SDL_Texture *textTargetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _w, _h);
	
	SDL_Rect srcRect, dstRect;
	srcRect.x = 0;
	srcRect.y = 6;
	srcRect.w = _w;
	srcRect.h = _h - srcRect.y;
	dstRect.x = 0;
	dstRect.y = 6;
	dstRect.w = _w;
	dstRect.h = _h;
	
	SDL_SetRenderTarget(renderer, textTargetTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	//SDL_RenderFillRect(renderer, NULL);
	SDL_SetTextureColorMod(texture, 200, 200, 200);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_SetTextureColorMod(texture, 255, 255, 255);
	SDL_RenderCopy(renderer, texture, &srcRect, &srcRect);
	SDL_SetRenderTarget(renderer, NULL);*/
	
	if (width)
		*width = _w;
	
	if (height)
		*height = _h;
	
	SDL_FreeSurface(surfaceMessage);
	
	//SDL_SetTextureColorMod(textTargetTexture, r, g, b);
	//SDL_SetTextureBlendMode(textTargetTexture, SDL_BLENDMODE_BLEND);
	
	return texture;
}
