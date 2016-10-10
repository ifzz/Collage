#include <SDL2/SDL.h>
#include <assert.h>
#include "framework/numbers.h"
#include "scene.h"
#include "system.h"
#include "camera.h"
#include "timestep.h"
#include "world.h"

float TIME = 0.;
int FRAMES, FPS_TIMER;
float DELTA_TIME;
float DELTA_TIME_STANDARD = 1 / 6.;
float NEXT_DELTA_TIME = 1 / 6.;
float NEXT_DELTA_TIME_MOD = .1;
float CURRENT_TIME = 0.;
float ACCUMULATOR = 0.;
float MAX_ACCUMULATOR = 0.;
float MIN_FRAME_TIME = 10000.;
float MAX_FRAME_TIME = 0;
float MAX_FRAMES = 0;
Delta* TIMESTEP_INFO;


void initTimestep() {
	CURRENT_TIME = SDL_GetTicks();
	FPS_TIMER = CURRENT_TIME;
	FRAMES = 0;

	createEvent(&EVENT_TIMESTEP);
	createEvent(&EVENT_TIMESTEP_START);
	createEvent(&EVENT_TIMESTEP_RENDER);
	createEvent(&EVENT_TICK);

	TIMESTEP_INFO = malloc(sizeof(Delta));

	DELTA_TIME = DELTA_TIME_STANDARD;
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

void tick(Delta *simulationInfo) {
	triggerEvents(EVENT_TIMESTEP, COMPONENT_STAGE, simulationInfo);
}

void update(Delta *simulationInfo) {
	triggerEvents(EVENT_TIMESTEP_START, COMPONENT_STAGE, simulationInfo);
}

void render(Delta *simulationInfo) {
	triggerEvents(EVENT_TIMESTEP_RENDER, COMPONENT_CAMERA, simulationInfo);
}

void manageTimestep() {
	if (DELTA_TIME != NEXT_DELTA_TIME) {
		DELTA_TIME = interpF(DELTA_TIME, NEXT_DELTA_TIME, NEXT_DELTA_TIME_MOD);
	}
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
		TIMESTEP_INFO->time = TIME;
		TIMESTEP_INFO->delta = DELTA_TIME;

		if (ACCUMULATOR > MAX_ACCUMULATOR)
			MAX_ACCUMULATOR = ACCUMULATOR;

		update(TIMESTEP_INFO);
		tick(TIMESTEP_INFO);
		cleanWorld();

		manageTimestep();
		TIME += DELTA_TIME;
		ACCUMULATOR -= DELTA_TIME;
	}

	float alpha = ACCUMULATOR / DELTA_TIME;

	Delta simulationInfo = {TIME, alpha};

	render(&simulationInfo);

	++ FRAMES;

	int currentTicks = SDL_GetTicks();

	if (currentTicks - FPS_TIMER >= 1000) {
		printf("FPS: %i\n", FRAMES);

		FRAMES = 0;
		FPS_TIMER = currentTicks;
	}
}	

void showTimestepInfo() {
	printf("**** TIMESTEP ****\n");
	printf("Longest frame: %f\n", MAX_FRAME_TIME);
	printf("Quickest frame: %f\n", MIN_FRAME_TIME);
	printf("Most frames: %f\n", MAX_FRAMES);
	printf("Max ACCU: %f\n", MAX_ACCUMULATOR);
}
