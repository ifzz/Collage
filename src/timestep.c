#include <SDL2/SDL.h>
#include "scene.h"
#include "system.h"
#include "timestep.h"

double TIME = 0.;
double DELTA_TIME = 1 / 6.;
double CURRENT_TIME = 0.;
double ACCUMULATOR = 0.;
double MAX_ACCUMULATOR = 0.;
double MIN_FRAME_TIME = 10000.;
double MAX_FRAME_TIME = 0;
double MAX_FRAMES = 0;


void initTimestep() {
	CURRENT_TIME = SDL_GetTicks();

	createEvent(&EVENT_TIMESTEP);
	createEvent(&EVENT_TIMESTEP_END);
}

void stepTime() {
	double newTime = SDL_GetTicks();
	double frameTime = (newTime - CURRENT_TIME) / 100.0f;

	if (frameTime > MAX_FRAME_TIME)
		MAX_FRAME_TIME = frameTime;

	if (frameTime && frameTime < MIN_FRAME_TIME)
		MIN_FRAME_TIME = frameTime;

	if (frameTime > .25)
		frameTime = .25;

	CURRENT_TIME = newTime;
	ACCUMULATOR += frameTime;

	while (ACCUMULATOR >= DELTA_TIME) {
		Delta timeInfo = {TIME, DELTA_TIME};

		if (ACCUMULATOR > MAX_ACCUMULATOR)
			MAX_ACCUMULATOR = ACCUMULATOR;

		triggerEvents(EVENT_TIMESTEP, 0, &timeInfo);

		TIME += DELTA_TIME;
		ACCUMULATOR -= DELTA_TIME;
	}

	const double alpha = 1 - (ACCUMULATOR / DELTA_TIME);

	Delta simulationInfo = {TIME, alpha};

	triggerEvents(EVENT_TIMESTEP_END, COMPONENT_SCENE, &simulationInfo);
}	

void showTimestepInfo() {
	printf("**** TIMESTEP ****\n");
	printf("Longest frame: %f\n", MAX_FRAME_TIME);
	printf("Quickest frame: %f\n", MIN_FRAME_TIME);
	printf("Most frames: %f\n", MAX_FRAMES);
	printf("Max ACCU: %f\n", MAX_ACCUMULATOR);
}
