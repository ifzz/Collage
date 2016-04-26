unsigned int COMPONENT_TIMER;
unsigned int EVENT_TIMER_TICK;
unsigned int EVENT_TIMER_EMIT;

void initTimer(void);
void registerTimer(unsigned int, int);

#ifndef TIMER_H
#define TIMER_H

typedef struct {
	int ticks, maxTicks, repeats;
	bool paused;
} TimerComponent;

#endif
