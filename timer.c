#include "component.h"
#include "timestep.h"
#include "entity.h"
#include "system.h"
#include "timer.h"

void eventTimerHandler(unsigned int, void*);

void initTimer() {
	addComponentToWorld(&COMPONENT_TIMER, sizeof(TimerComponent));

	createEvent(&EVENT_TIMER_TICK);
	createEvent(&EVENT_TIMER_EMIT);

	createSystem(EVENT_TIMESTEP, COMPONENT_TIMER, eventTimerHandler);
}

void registerTimer(unsigned int entityId, int ticks) {
	addComponentToEntity(entityId, COMPONENT_TIMER);

	TimerComponent *timer = getComponent(entityId, COMPONENT_TIMER);

	timer->ticks = 0;
	timer->maxTicks = ticks;
}

void eventTimerHandler(unsigned int entityId, void *data) {
	TimerComponent *timer = getComponent(entityId, COMPONENT_TIMER);

	if (timer->paused)
		return;

	if (timer->ticks < timer->maxTicks) {
		++ timer->ticks;

		triggerEvent(entityId, EVENT_TIMER_TICK, &timer);

		return;
	}

	triggerEvent(entityId, EVENT_TIMER_EMIT, &timer);

	if (timer->repeats) {
		timer->ticks = 0;

		-- timer->repeats;
	} else
		timer->paused = true;
}
