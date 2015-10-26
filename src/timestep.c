#include <stdio.h>
#include <time.h>
#include "system.h"
#include "timestep.h"

double TIME = 0.;
double DELTA_TIME = 0.01;
double CURRENT_TIME = 0.;
double ACCUMULATOR = 0.;
unsigned int EVENT_TIMESTEP = 0;

typedef struct {
	double time;
	double delta;
} Delta;

int getHiResTime() {
	struct timespec hiTime;
    
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &hiTime);

	return hiTime.tv_nsec;
}

void initTimestep() {
	CURRENT_TIME = getHiResTime();

	createEvent(&EVENT_TIMESTEP);
}

void stepTime() {
	double newTime = getHiResTime();
	double frameTime = newTime - CURRENT_TIME;

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

}	
