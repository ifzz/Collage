#include <SDL2/SDL.h>
#include "scene.h"
#include "system.h"
#include "timestep.h"

double TIME = 0.;
double DELTA_TIME = 1 / 60.;
double CURRENT_TIME = 0.;
double ACCUMULATOR = 0.;


void initTimestep() {
	CURRENT_TIME = SDL_GetTicks();

	createEvent(&EVENT_TIMESTEP);
	createEvent(&EVENT_TIMESTEP_END);
}

void stepTime() {
	double newTime = SDL_GetTicks();
	double frameTime = (newTime - CURRENT_TIME) / 1000.0f;

	if (frameTime > .25)
		frameTime = .25;

	CURRENT_TIME = newTime;
	ACCUMULATOR += frameTime;

	while (ACCUMULATOR >= DELTA_TIME) {
		Delta timeInfo = {TIME, DELTA_TIME};

		triggerEvents(EVENT_TIMESTEP, 0, &timeInfo);

		TIME += DELTA_TIME;
		ACCUMULATOR -= DELTA_TIME;
	}

	const double alpha = ACCUMULATOR / DELTA_TIME;
	Delta simulationInfo = {TIME, alpha};

	triggerEvents(EVENT_TIMESTEP_END, COMPONENT_SCENE, &simulationInfo);
}	
