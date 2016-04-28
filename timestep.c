#include <SDL2/SDL.h>
#include "framework/numbers.h"
#include "scene.h"
#include "system.h"
#include "timestep.h"
#include "world.h"

float TIME = 0.;
float DELTA_TIME = 1 / 6.;
const float DELTA_TIME_STANDARD = 1 / 6.;
float NEXT_DELTA_TIME = 1 / 6.;
float NEXT_DELTA_TIME_MOD = .1;
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
	createEvent(&EVENT_TIMESTEP_RENDER);
	createEvent(&EVENT_TICK);
}

void resetTimestep() {
	CURRENT_TIME = SDL_GetTicks();
}

void setTimestepModifier(float mod, float rate) {
	if (rate == 1.) {
		DELTA_TIME = DELTA_TIME_STANDARD * mod;
	}

	NEXT_DELTA_TIME = DELTA_TIME_STANDARD * mod;
	NEXT_DELTA_TIME_MOD = rate;
}

void update(Delta simulationInfo) {
	triggerEvents(EVENT_TIMESTEP_END, COMPONENT_STAGE, &simulationInfo);
}

void render(Delta simulationInfo) {
	triggerEvents(EVENT_TIMESTEP_RENDER, COMPONENT_STAGE, &simulationInfo);
}

void manageTimestep() {
	if (DELTA_TIME != NEXT_DELTA_TIME)
		DELTA_TIME = interp(DELTA_TIME, NEXT_DELTA_TIME, NEXT_DELTA_TIME_MOD);
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

		manageTimestep();
		triggerEventForAll(EVENT_TICK, 0, &timeInfo);
		triggerEvents(EVENT_TIMESTEP, 0, &timeInfo);

		TIME += DELTA_TIME;
		ACCUMULATOR -= DELTA_TIME;
	}

	const float alpha = ACCUMULATOR / DELTA_TIME;

	Delta simulationInfo = {TIME, alpha};

	update(simulationInfo);
	render(simulationInfo);
	cleanWorld();
}	

void showTimestepInfo() {
	printf("**** TIMESTEP ****\n");
	printf("Longest frame: %f\n", MAX_FRAME_TIME);
	printf("Quickest frame: %f\n", MIN_FRAME_TIME);
	printf("Most frames: %f\n", MAX_FRAMES);
	printf("Max ACCU: %f\n", MAX_ACCUMULATOR);
}
