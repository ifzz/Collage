#include <stdio.h>
#include <assert.h>
#include "world.h"
#include "entity.h"


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

	return returnedId;
}

void deleteEntity(unsigned int entityId) {
	World *world = getWorld();

	world->entityMask[entityId] = 0;
	world->entityIdsToDelete[world->deletedEntityCount] = entityId;

	++ world->entityIdsToDeleteCount;
	
	printf("[ENTITY] Deleted entity #%u\n", entityId);
}

void registerEntityEvent(unsigned int entityId, unsigned int eventId, void (*callback)(unsigned int, void*)) {
	World *world = getWorld();
	int callbackCount = world->entityEventCallbackCount[entityId][eventId];

	world->entityEventCallback[entityId][eventId][callbackCount] = callback;
	++ world->entityEventCallbackCount[entityId][eventId];

}

