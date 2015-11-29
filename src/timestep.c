#include <SDL2/SDL.h>
#include "scene.h"
#include "system.h"
#include "timestep.h"

float TIME = 0.;
float DELTA_TIME = 1 / 6.;
float CURRENT_TIME = 0.;
float ACCUMULATOR = 0.;
float MAX_ACCUMULATOR = 0.;
float MIN_FRAME_TIME = 10000.;
float MAX_FRAME_TIME = 0;
float MAX_FRAMES = 0;


void initTimestep() {
	CURRENT_TIME = SDL_GetTicks();

	createEvent(&EVENT_TIMESTEP);
	createEvent(&EVENT_TIMESTEP_END);
	createEvent(&EVENT_TICK);
}

void resetTimestep() {
	CURRENT_TIME = SDL_GetTicks();
}

void stepTime() {
	float newTime = SDL_GetTicks();
	float frameTime = (newTime - CURRENT_TIME) / 100.0f;

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

		triggerEventForAll(EVENT_TICK, 0, &timeInfo);
		triggerEvents(EVENT_TIMESTEP, 0, &timeInfo);

		TIME += DELTA_TIME;
		ACCUMULATOR -= DELTA_TIME;
	}

	const float alpha = 1 - (ACCUMULATOR / DELTA_TIME);

	Delta simulationInfo = {TIME, alpha};

	triggerEvents(EVENT_TIMESTEP_END, COMPONENT_STAGE, &simulationInfo);
}	

void showTimestepInfo() {
	printf("**** TIMESTEP ****\n");
	printf("Longest frame: %f\n", MAX_FRAME_TIME);
	printf("Quickest frame: %f\n", MIN_FRAME_TIME);
	printf("Most frames: %f\n", MAX_FRAMES);
	printf("Max ACCU: %f\n", MAX_ACCUMULATOR);
}
