#include <stdio.h>
#include <assert.h>
#include "world.h"
#include "entity.h"
#include "system.h"

int delCount = 0;

void initEntities() {
	createEvent(&EVENT_DELETE);
}

unsigned int createEntity() {
	World *world = getWorld();
	int returnedId = -1;

	if (world->deletedEntityCount) {
		returnedId = world->deletedEntityIds[world->deletedEntityCount - 1];

		-- world->deletedEntityCount;
	} else if (world->entityCount < world->entityCountMax) {
		returnedId = world->entityCount;
		++ world->entityCount;
	}

	if (returnedId == -1) {
		printf("[ENTITY-#FATAL] Cannot return new entity ID\n");
		
		assert(1 == 2);
	}

	/*printf("[ENTITY] Created new entity: %i\n", returnedId);*/

	if (returnedId > world->highestEntityId)
		world->highestEntityId = returnedId;

	return returnedId;
}

void deleteEntity(unsigned int entityId) {
	World *world = getWorld();

	triggerEvent(entityId, EVENT_DELETE, world);

	world->entityIdsToDelete[world->entityIdsToDeleteCount] = entityId;

	++ world->entityIdsToDeleteCount;
	
	printf("[ENTITY] Deleted entity #%u\n", entityId);

	/*if (delCount == 1)*/
		/*assert ( 1 == 2);*/

	/*++ delCount;*/
}

void registerEntityEvent(unsigned int entityId, unsigned int eventId,
		void (*callback)(unsigned int, void*)) {
	World *world = getWorld();
	int callbackCount = world->entityEventCallbackCount[entityId][eventId];

	world->entityEventCallback[entityId][eventId][callbackCount] = callback;
	++ world->entityEventCallbackCount[entityId][eventId];

}

void triggerEvent(unsigned int entityId, unsigned int eventId, void *data) {
	World *world = getWorld();
	unsigned int entityMask = world->entityMask[entityId];

	for (unsigned int i = 0; i < world->systemIndex[eventId]; ++ i) {
		unsigned int systemMask = world->systemMask[eventId][i];

		if ((entityMask & systemMask) == systemMask) {
			world->systemCallback[eventId][i](entityId, data);
		}
	}

	for (unsigned int i = 0;
			i < world->entityEventCallbackCount[entityId][eventId];
			++ i) {
		world->entityEventCallback[entityId][eventId][i](entityId, data);
	}
}

