unsigned int EVENT_TIMESTEP;
unsigned int EVENT_TIMESTEP_END;

#ifndef TIMESTEP_H
#define TIMESTEP_H

typedef struct {
	double time;
	double delta;
} Delta;

#endif

void initTimestep(void);
void stepTime(void);
void showTimestepInfo(void);

