#include <stdio.h>
#include <assert.h>
#include "world.h"
#include "entity.h"
#include "system.h"
#include "debug.h"

void initEntities() {
	createEvent(&EVENT_DELETE);
	createEvent(&EVENT_CLEANUP);
}

unsigned int createEntity() {
	World *world = getWorld();
	int returnedId = -1;

	if (world->deletedEntityCount) {
		returnedId = world->deletedEntityIds[world->deletedEntityCount - 1];

		/*logInfo("Using deleted entity id=%i, remaining=%i", returnedId, world->deletedEntityCount);*/

		-- world->deletedEntityCount;
	} else if (world->entityCount < world->entityCountMax) {
		returnedId = world->entityCount;
		++ world->entityCount;

		/*logInfo("Using new entity id=%i, total count: %i", returnedId, world->entityCount);*/
	}

	if (returnedId == -1) {
		logError("[ENTITY-#FATAL] Cannot return new entity ID");
		
		assert(1 == 2);
	}

	
	assert(returnedId < world->entityCountMax);

	/*printf("[ENTITY] Created new entity: %i\n", returnedId);*/

	if (returnedId > world->highestEntityId)
		world->highestEntityId = returnedId;

	return returnedId;
}

void deleteEntity(unsigned int entityId) {
	World *world = getWorld();

#if defined(DEBUG)
		/*for (int i = 0; i < world->entityIdsToDeleteCount; ++ i)*/
			/*assert(entityId != world->entityIdsToDelete[i]);*/
#else
#endif
	bool foundDupe = false;

	for (int i = 0; i < world->entityIdsToDeleteCount && !foundDupe; ++ i)
		foundDupe = entityId == world->entityIdsToDelete[i];

	if (foundDupe) {
		return;
	}

	triggerEvent(entityId, EVENT_CLEANUP, world);

	/*world->entityMask[entityId] = 0;*/

	/*for (int j = 0; j < world->eventCount; ++ j)*/
		/*world->entityEventCallbackCount[entityId][j] = 0;*/
	
	/*world->deletedEntityIds[world->deletedEntityCount] = entityId;*/

	/*++ world->deletedEntityCount;*/
	world->entityIdsToDelete[world->entityIdsToDeleteCount] = entityId;

	++ world->entityIdsToDeleteCount;
	
	/*logInfo("[ENTITY] Deleted entity #%u", entityId);*/
}

void registerEntityEvent(unsigned int entityId, unsigned int eventId,
		void (*callback)(unsigned int, void*)) {
	World *world = getWorld();
	int callbackCount = world->entityEventCallbackCount[entityId][eventId];

	world->entityEventCallback[entityId][eventId][callbackCount] = callback;
	++ world->entityEventCallbackCount[entityId][eventId];
}

void unregisterEntityEvent(unsigned int entityId, unsigned int eventId,
		void (*callback)(unsigned int, void*)) {
	World *world = getWorld();
	int callbackCount = world->entityEventCallbackCount[entityId][eventId];
	bool shrinking = false;

	for (int i = 0; i < callbackCount; ++ i) {
		if (callback == world->entityEventCallback[entityId][eventId][i])
			shrinking = true;

		if (shrinking)
			world->entityEventCallback[entityId][eventId][i] =
				world->entityEventCallback[entityId][eventId][i + 1];
	}

	assert(shrinking);

	-- world->entityEventCallbackCount[entityId][eventId];
}

void unregisterAllEntityEvents(unsigned int entityId, unsigned int eventId) {
	World *world = getWorld();

	world->entityEventCallbackCount[entityId][eventId] = 0;
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

	for (int i = 0;
			i < world->entityEventCallbackCount[entityId][eventId];
			++ i) {
		world->entityEventCallback[entityId][eventId][i](entityId, data);
	}
}

