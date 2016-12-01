unsigned int EVENT_TIMESTEP;
unsigned int EVENT_TIMESTEP_START;
unsigned int EVENT_TIMESTEP_RENDER;
unsigned int EVENT_TICK;

#ifndef TIMESTEP_H
#define TIMESTEP_H

typedef struct {
	double time;
	double delta;
} Delta;

#endif

void initTimestep(void);
void setTimestepModifier(float, float);
void resetTimestep(void);
void stepTime(void);
void showTimestepInfo(void);
int getTimestepTicks(void);
void increaseTimestepAccel(void);
void decreaseTimestepAccel(void);
int getFps(void);
