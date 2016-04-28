#include <assert.h>
#include "component.h"
#include "timestep.h"
#include "entity.h"
#include "system.h"
#include "timer.h"

void eventAddTimerHandler(unsigned int, void*);
void eventTickTimerHandler(unsigned int, void*);

void initTimer() {
	addComponentToWorld(&COMPONENT_TIMER, sizeof(TimerComponent));

	createEvent(&EVENT_TIMER_TICK);
	createEvent(&EVENT_TIMER_EMIT);
	createEvent(&EVENT_ADD_TIMER);

	createSystem(EVENT_TIMESTEP, COMPONENT_TIMER, eventTickTimerHandler);
	createSystem(EVENT_ADD_TIMER, COMPONENT_TIMER, eventAddTimerHandler);
}

void registerTimer(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_TIMER);

	TimerComponent *timer = getComponent(entityId, COMPONENT_TIMER);

	for (int i = 0; i < MAX_TIMER_COUNT; ++ i) {
		Timer *oldTimer = &timer->timers[i];

		oldTimer->active = false;
	}
}

bool hasTimer(unsigned int entityId, char *name) {
	TimerComponent *timer = getComponent(entityId, COMPONENT_TIMER);

	for (int i = 0; i < MAX_TIMER_COUNT; ++ i) {
		Timer *oldTimer = &timer->timers[i];

		if (!oldTimer->active)
			continue;

		if (!strcmp(oldTimer->name, name))
			return true;
	}

	return false;
}

void eventAddTimerHandler(unsigned int entityId, void *data) {
	TimerComponent *timer = getComponent(entityId, COMPONENT_TIMER);
	AddTimerEvent *newTimer = (AddTimerEvent*)data;

	for (int i = 0; i < MAX_TIMER_COUNT; ++ i) {
		Timer *oldTimer = &timer->timers[i];

		if (!oldTimer->active) {
			snprintf(oldTimer->name, MAX_TIMER_NAME_LEN, "%s", newTimer->name);

			oldTimer->ticks = 0;
			oldTimer->maxTicks = newTimer->time;
			oldTimer->repeats = newTimer->repeats;
			oldTimer->active = true;
			oldTimer->paused = false;

			printf("Created new timer: %s\n", newTimer->name);

			return;
		}
	}

	assert(1 == 2 && "Exceeded max timer count for entity");
}

void eventTickTimerHandler(unsigned int entityId, void *data) {
	TimerComponent *timerComp = getComponent(entityId, COMPONENT_TIMER);

	for (int i = 0; i < MAX_TIMER_COUNT; ++ i) {
		Timer *timer = &timerComp->timers[i];

		if (!timer->active || timer->paused)
			continue;

		if (timer->ticks < timer->maxTicks) {
			++ timer->ticks;

			triggerEvent(entityId, EVENT_TIMER_TICK, timer);

			continue;
		}

		triggerEvent(entityId, EVENT_TIMER_EMIT, timer);

		if (timer->repeats) {
			timer->ticks = 0;

			if (timer->repeats != -1)
				-- timer->repeats;
		} else
			timer->paused = true;
	}
}
