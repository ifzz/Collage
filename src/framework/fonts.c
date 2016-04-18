#include <SDL2/SDL_ttf.h>
#include <stdio.h>
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
	
	return NULL;
}

TTF_Font *addFontWithNameAndSize(char *filename, char *name, int size) {
	fontManagerEntry *fontEntry = malloc(sizeof(fontManagerEntry));
	
	fontEntry->name = malloc(strlen(name) + 1);
	
	strcpy(fontEntry->name, name);
	
	fontEntry->font = TTF_OpenFont(filename, size);
	fontEntry->size = size;
	
	addListItem(FONT_MANAGER, fontEntry);
	
	printf("Added new font: %s, size=%i\n", name, size);
	
	return fontEntry->font;
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
	
	if (width)
		*width = _w;
	
	if (height)
		*height = _h;
	
	SDL_FreeSurface(surfaceMessage);
	
	return texture;
}
