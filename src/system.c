#include <stdio.h>
#include "world.h"
#include "system.h"


void createSystem(unsigned int eventId, unsigned int componentMask, void (*callback)(unsigned int, void*)) {
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
}

//#TODO: Move to entity.c
void triggerEvent(unsigned int entityId, unsigned int eventId, void *data) {
	World *world = getWorld();
	unsigned int entityMask = getWorld()->entityMask[entityId];

	for (unsigned int i = 0; i < world->systemIndex[eventId]; ++ i) {
		unsigned int systemMask = world->systemMask[eventId][i];

		if ((entityMask & systemMask) == systemMask) {
			world->systemCallback[eventId][i](entityId, data);
		}
	}

	for (unsigned int i = 0; i < world->entityEventCallbackCount[entityId][eventId]; ++ i) {
		world->entityEventCallback[entityId][eventId][i](entityId, data);
	}
}

//#TODO: Rename: triggerSystemEvent
void triggerEvents(unsigned int eventId, unsigned int componentFlags, void *data) {
	World *world = getWorld();

	//#TODO: Can we cache event->entityIds?
	for (unsigned int entityId = 0; entityId < world->entityCount; ++ entityId) {
		unsigned int entityMask = getWorld()->entityMask[entityId];

		if ((entityMask & componentFlags) != componentFlags)
			continue;

		for (unsigned int i = 0; i < world->systemIndex[eventId]; ++ i) {
			unsigned int systemMask = world->systemMask[eventId][i];
			
			//#TODO Should we even check for this?
			if ((systemMask & componentFlags) == componentFlags) {
				world->systemCallback[eventId][i](entityId, data);
			}
		}
		
	}
}

