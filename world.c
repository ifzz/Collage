#include "framework/list.h"
#include "framework/strings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "world.h"
#include "component.h"
#include "system.h"
#include "entity.h"
#include "debug.h"


void deleteWorld(void*);

World *ACTIVE_WORLD = NULL;
World *WORLD_LIST[5];
int WORLD_COUNT = 0;
/*linkedList_t *WORLD_LIST = NULL;*/

int MAX_EVENTS = 70;

void initWorlds() {
	/*WORLD_LIST = createLinkedList(&deleteWorld);*/

	/*assert(1 == 2);*/
}

void destroyWorlds() {
	/*deleteLinkedList(WORLD_LIST);*/
}

void createWorld(char *name) {
	World *newWorld = calloc(1, sizeof(World));

	//#TODO: This should be MAX_EVENTS.
	int maxEvents = MAX_EVENTS;
	int entityEventCallbackCountMax = 100;

	newWorld->entityCount = 0;
	newWorld->componentCount = 0;
	newWorld->entityCountMax = 3000;
	newWorld->systemCountMax = MAX_EVENTS;
	newWorld->entityMask = calloc(newWorld->entityCountMax, sizeof(unsigned int));
	newWorld->components = calloc(40, sizeof(void*));
	/*newWorld->componentNames = calloc(MAX_COMPONENT_NAME_LEN, sizeof(char));*/
	newWorld->entityEventCallbackCount = calloc(newWorld->entityCountMax, sizeof(int*));
	newWorld->entityIdsToDeleteCount = 0;
	newWorld->deletedEntityCount = 0;
	newWorld->highestEntityId = -1;

	/*printf("\n\nREPLACE THESE WITH ACTUAL VALUES!!!!!!!!!\n\n");*/

	/*for (int e = 0; e < 40; ++ e)*/
		/*newWorld->components[e] = calloc(newWorld->entityCountMax, sizeof(ComponentContainer));*/
	
	for (int e = 0; e < newWorld->entityCountMax; ++ e)
		newWorld->entityEventCallbackCount[e] = calloc(entityEventCallbackCountMax, sizeof(int));
	
	newWorld->systemIndex = calloc(newWorld->entityCountMax, sizeof(unsigned int));
	newWorld->systemMask = calloc(newWorld->systemCountMax, sizeof(unsigned int*));

	for (int i = 0; i < newWorld->systemCountMax; ++ i)
		newWorld->systemMask[i] = calloc(newWorld->systemCountMax, sizeof(unsigned int));

	newWorld->systemCallback = calloc(newWorld->systemCountMax, sizeof(void (**)(unsigned int, void*)));
	
	for (int i = 0; i < newWorld->systemCountMax; ++ i)
		newWorld->systemCallback[i] = calloc(newWorld->systemCountMax, sizeof(void (*)(unsigned int, void*)));
	
	newWorld->entityEventCallback = calloc(newWorld->entityCountMax, sizeof(void (***)(unsigned int, void*)));

	for (int e = 0; e < newWorld->entityCountMax; ++ e) {
		newWorld->entityEventCallback[e] = calloc(maxEvents, sizeof(void (**)(unsigned int, void*))); //#TODO: MAX_EVENTS

		for (int i = 0; i < maxEvents; ++ i) {
			newWorld->entityEventCallback[e][i] = calloc(entityEventCallbackCountMax, sizeof(void (*)(unsigned int, void*))); //#TODO: MAX_ENTITY_EVENT_CALLBACKS
		}
	}

	newWorld->entityIdsToDelete = calloc(newWorld->entityCountMax, sizeof(unsigned int));
	newWorld->deletedEntityIds = calloc(newWorld->entityCountMax, sizeof(unsigned int));
	newWorld->systems = calloc(newWorld->entityCountMax, sizeof(void*));

	snprintf(newWorld->name, 25, "%s", name);

	logInfo("[WORLD] Created new world: %s", name);
	
	WORLD_LIST[WORLD_COUNT] = newWorld;
	++ WORLD_COUNT;
	/*addListItem(WORLD_LIST, newWorld);*/
}

void cleanWorld() {
	World *world = ACTIVE_WORLD;

	if (world->entityIdsToDeleteCount == 0)
		return;

	logInfo("[WORLD-CLEANING] Number of entities to clean: %i", world->entityIdsToDeleteCount);

	for (int i = 0; i < world->entityIdsToDeleteCount; ++ i) {
		unsigned int entityId = world->entityIdsToDelete[i];

		logInfo("[WORLD-CLEANING] Cleaning entity: %i", entityId);

		/*assert(1 == 2);*/
		triggerEvent(entityId, EVENT_DELETE, world);

		for (int j = 0; j < world->eventCount; ++ j)
			world->entityEventCallbackCount[entityId][j] = 0;

#if defined(DEBUG)
		/*assert(world->entityMask[entityId] > 0);*/
#else
		if (world->entityMask[entityId] == 0)
			puts("Error: Trying to delete empty entity");
#endif

		world->entityMask[entityId] = 0;
		world->deletedEntityIds[world->deletedEntityCount] = entityId;

		++ world->deletedEntityCount;
		/*-- world->entityCount;*/
	}

	world->entityIdsToDeleteCount = 0;

	logInfo("[WORLD-CLEANING] Clean");
}

void deleteWorld(void *data) {
	World *world = data;
	int maxEvents = MAX_EVENTS;
	
	for (int i = 0; i < world->componentCount; ++ i)
		removeComponentFromWorld(i);

	for (int i = 0; i < world->systemCountMax; ++ i)
		free(world->systemMask[i]);

	for (int i = 0; i < world->systemCountMax; ++ i)
		free(world->systemCallback[i]);
	
	for (int e = 0; e < world->entityCountMax; ++ e) {
		for (int i = 0; i < maxEvents; ++ i) {
			free(world->entityEventCallback[e][i]);
		}

		free(world->entityEventCallback[e]);
	}

	free(world->entityEventCallback);

	for (int e = 0; e < world->entityCountMax; ++ e)
		free(world->entityEventCallbackCount[e]);

	free(world->entityEventCallbackCount);
	//#TODO: Clean up entityEvents

	free(world->name);
	free(world->systems);
	free(world->entityMask);
	free(world->components);
	free(world->systemIndex);
	free(world->systemMask); 
	free(world->systemCallback); 
	free(world->entityIdsToDelete);
	free(world->deletedEntityIds);
}


World* getWorld() {
	return ACTIVE_WORLD;
}

void setWorld(char *name) {
	/*for (listItem_t *item = WORLD_LIST->head; item; item = item->next) {*/
	for (int i = 0; i < WORLD_COUNT; ++ i) {
		/*printf("Looking for world: %s\n", name);*/
		World *world = WORLD_LIST[i];
		/*World *world = item->item;*/
		
		if (!strcmp(world->name, name)) {
			ACTIVE_WORLD = world;

			logInfo("[WORLD] Active World changed to: %s", name);

			return;
		}
	}

	logError("[WORLD-#FATAL] Cannot get world: %s", name);

	assert(NULL);
}

void killWorld() {
	deleteListItem(WORLD_LIST, ACTIVE_WORLD);

	ACTIVE_WORLD = NULL;
}
