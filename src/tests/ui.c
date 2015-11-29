#include <SDL2/SDL.h>
#include "../framework/display.h"
#include "../entity.h"
#include "../scene.h"
#include "../component.h"


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

void createUi() {
	unsigned int uiEntityId = createEntity();

	addEntityToScene("ui", uiEntityId);
	registerEntityEvent(uiEntityId, EVENT_DRAW, &drawUi);
}
