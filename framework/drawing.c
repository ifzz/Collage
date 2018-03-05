#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string.h>
#include <assert.h>
#include "../debug.h"
#include "list.h"
#include "display.h"
#include "drawing.h"
#include "numbers.h"


typedef struct fontManagerEntry {
		char name[MAX_FONT_NAME_LEN];
		TTF_Font *font;
		int size;
} fontManagerEntry;

linkedList_t *FONT_MANAGER = NULL;


void drawPixel(Uint32 *pixels, int x, int y, int w, Uint32 pixel){
    pixels[(y * w) + x] = pixel;
}

SDL_Texture* textureGenerateTile(SDL_Renderer *renderer,
		SDL_Texture *sourceTexture,
		int width, int height) {
	int sourceTextureWidth, sourceTextureHeight;
	SDL_Texture *targetTexture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height);

	SDL_SetRenderTarget(renderer, targetTexture); 
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_QueryTexture(sourceTexture, NULL, NULL, &sourceTextureWidth,
			&sourceTextureHeight);
	SDL_SetTextureBlendMode(sourceTexture, SDL_BLENDMODE_BLEND);
	SDL_Rect srcRect = {0, 0, sourceTextureWidth, sourceTextureHeight};

	for (int w = 0; w < width; w += sourceTextureWidth) {
		for (int h = 0; h < height; h += sourceTextureHeight) {
			SDL_Rect dstRect = {w, h, sourceTextureWidth, sourceTextureHeight};
			SDL_RenderCopy(renderer, sourceTexture, &srcRect, &dstRect);
		}
	}

	displayResetRenderTarget(renderer);
	
	return targetTexture;
}

void deleteFontFromList(void *data) {
	fontManagerEntry *fME = data;

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

	snprintf(fontEntry->name, MAX_FONT_NAME_LEN, "%s", name);

	fontEntry->font = TTF_OpenFont(filename, size);

	if (fontEntry->font == NULL)
		assert(1 == 2);

	fontEntry->size = size;

	addListItem(FONT_MANAGER, fontEntry);

	logInfo("Added new font: %s, size=%i\n", name, size);

	return fontEntry->font;
}

SDL_Texture *renderFont(SDL_Renderer *renderer, char *fontName, int r, int g,
		int b, int size, int *width, int *height, char *text) {
	TTF_Font *font = getFontWithNameAndSize(fontName, size);

	if (font == NULL)
		font = addFontWithNameAndSize(fontName, fontName, size);

	SDL_Color color = {r, g, b, 255};
	int _w, _h;

	SDL_Surface *surfaceMessage = TTF_RenderText_Blended(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_QueryTexture(texture, NULL, NULL, &_w, &_h);

	if (width != NULL)
		*width = _w;

	if (height != NULL)
		*height = _h;

	SDL_FreeSurface(surfaceMessage);

	return texture;
}

void drawRotatedBox(SDL_Renderer *renderer, int rot, int pnts[][2],
		int pntCnt) {
	//TODO: Broken
	SDL_Point points[255];

	assert(pntCnt < 255);

	for (int i = 0; i < pntCnt; ++ i) {
		SDL_Point *pnt = &points[i];
		double point[2] = {pnts[i][0], pnts[i][1]};

		vectorRotate(point, point, rot);

		pnt->x = round(point[0]);
		pnt->y = round(point[1]);
	}

	SDL_RenderDrawLines(renderer, points, pntCnt);
}

void drawText(SDL_Renderer *renderer, char *text, int x, int y, bool centered,
		int r, int g, int b, int a) {
	if (centered)
		x -= strlen(text) * 6;

	stringRGBA(renderer, x, y, text, r, g, b, a);
}
