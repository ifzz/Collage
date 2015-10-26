#include <stdio.h>
#include "world.h"
#include "system.h"

void createSystem(unsigned int eventId, unsigned int componentMask, void (*callback)(void*)) {
	World *world = getWorld();

	world->systemMask[eventId][world->systemIndex[eventId]] = componentMask;
	world->systemCallback[eventId][world->systemIndex[eventId]] = callback;
	
	printf("[SYSTEM] Created callback for event ID=%u at INDEX=%i\n", eventId, world->systemIndex[eventId]);

	++ world->systemIndex[eventId];
}

void createEvent(unsigned int *eventId) {
	World *world = getWorld();

	world->systemIndex[world->eventCount] = 0;
	*eventId = world->eventCount;

	++ world->eventCount;

	//world->eventMask[world->eventCount] = :
}

void triggerEvent(unsigned int entityId, unsigned int eventId, void *data) {
	World *world = getWorld();
	unsigned int entityMask = getWorld()->entityMask[entityId];

	for (int i = 0; i < world->systemIndex[eventId]; ++ i) {
		unsigned int systemMask = world->systemMask[eventId][i];

		if ((entityMask & systemMask) == systemMask) {
			world->systemCallback[eventId][i](data);
		}
	}
}
