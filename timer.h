unsigned int COMPONENT_TIMER;
unsigned int EVENT_TIMER_TICK;
unsigned int EVENT_TIMER_EMIT;
unsigned int EVENT_TIMER_FINISH;
unsigned int EVENT_ADD_TIMER;

void initTimer(void);
void registerTimer(unsigned int);
bool hasTimer(unsigned int, char*);

#ifndef TIMER_H
#define TIMER_H

#define MAX_TIMER_COUNT 12
#define MAX_TIMER_NAME_LEN 24

typedef struct {
	char name[MAX_TIMER_NAME_LEN];
	int ticks, maxTicks, repeats, maxRepeats;
	bool paused, active;
	void (*callback)(unsigned int, void*);
	void *data;
} Timer;

typedef struct {
	Timer timers[MAX_TIMER_COUNT];
} TimerComponent;

typedef struct {
	char name[MAX_TIMER_NAME_LEN];
	int time, repeats;
	void (*callback)(unsigned int, void*);
	void *data;
} AddTimerEvent;

#endif
