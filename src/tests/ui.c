#include <SDL2/SDL.h>
#include "../framework/display.h"
#include "../framework/drawing.h"
#include "../framework/sprite.h"
#include "../entity.h"
#include "../scene.h"
#include "../component.h"

SDL_Texture *BACKGROUND_TEXTURE = NULL;


void drawUi(unsigned int entityId, void *data) {
	SDL_Renderer *renderer = displayGetRenderer();
	DrawEvent *drawEvent = (DrawEvent*)data;
	SDL_Point lines[5];

	lines[0].x = 8;
	lines[0].y = 8;
	lines[1].x = displayGetRenderWidth() - 8;
	lines[1].y = 8;
	lines[2].x = displayGetRenderWidth() - 8;
	lines[2].y = displayGetRenderHeight() - 8;
	lines[3].x = 8;
	lines[3].y = displayGetRenderHeight() - 8;
	lines[4].x = 8;
	lines[4].y = 8;

	SDL_SetRenderDrawColor(renderer, 0, 93, 255, 255);
	SDL_RenderDrawLines(renderer, lines, 5);
}

void drawBackground(unsigned int entityId, void *data) {
	SDL_Renderer *renderer = displayGetRenderer();
	DrawEvent *drawEvent = (DrawEvent*)data;

	textureRender(renderer, BACKGROUND_TEXTURE, 8, 8);
}

void uiRenderBackground() {
	SDL_Renderer *renderer = displayGetRenderer();

	if (BACKGROUND_TEXTURE != NULL) {
		SDL_DestroyTexture(BACKGROUND_TEXTURE);

		BACKGROUND_TEXTURE = NULL;
	}

	SDL_Texture *tileTexture =
		textureCreate("src/tests/data/sprites/background.png");
	BACKGROUND_TEXTURE = textureGenerateTile(renderer,
			tileTexture, displayGetRenderWidth() - 16,
			displayGetRenderHeight() - 16);

}

void createUi() {
	unsigned int uiEntityId = createEntity();
	unsigned int backgroundEntityId = createEntity();

	uiRenderBackground();

	addEntityToScene("ui", uiEntityId);
	addEntityToScene("background", backgroundEntityId);

	registerEntityEvent(uiEntityId, EVENT_DRAW, &drawUi);
	registerEntityEvent(backgroundEntityId, EVENT_DRAW, &drawBackground);
}

