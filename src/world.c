#include "framework/list.h"
#include "framework/strings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "world.h"
#include "component.h"
#include "system.h"


void deleteWorld(void*);

World *ACTIVE_WORLD = NULL;
linkedList_t *WORLD_LIST = NULL;

int MAX_EVENTS = 30;

void initWorlds() {
	WORLD_LIST = createLinkedList(&deleteWorld);
}

void destroyWorlds() {
	deleteLinkedList(WORLD_LIST);
}

void createWorld(char *name) {
	World *newWorld = calloc(sizeof(World), 1);

	//#TODO: This should be MAX_EVENTS.
	int maxEvents = MAX_EVENTS;
	int entityEventCallbackCountMax = 100;

	newWorld->entityCount = 0;
	newWorld->entityCountMax = 2000;
	newWorld->systemCountMax = 10;
	newWorld->entityMask = calloc(sizeof(unsigned int), newWorld->entityCountMax);
	newWorld->components = calloc(sizeof(void*), newWorld->entityCountMax);
	newWorld->entityEventCallbackCount = calloc(sizeof(unsigned int*), newWorld->entityCountMax);
	
	for (int e = 0; e < newWorld->entityCountMax; ++ e)
		newWorld->entityEventCallbackCount[e] = calloc(sizeof(int), entityEventCallbackCountMax);
	
	newWorld->systemIndex = calloc(sizeof(unsigned int), newWorld->entityCountMax);
	newWorld->systemMask = calloc(sizeof(unsigned int*), newWorld->systemCountMax);

	for (int i = 0; i < newWorld->systemCountMax; ++ i)
		newWorld->systemMask[i] = calloc(sizeof(unsigned int), newWorld->systemCountMax);

	newWorld->systemCallback = calloc(sizeof(void (**)(unsigned int, void*)), newWorld->systemCountMax);
	
	for (int i = 0; i < newWorld->systemCountMax; ++ i)
		newWorld->systemCallback[i] = calloc(sizeof(void (*)(unsigned int, void*)), newWorld->systemCountMax);
	
	newWorld->entityEventCallback = calloc(sizeof(void (***)(unsigned int, void*)), newWorld->entityCountMax);

	for (int e = 0; e < newWorld->entityCountMax; ++ e) {
		newWorld->entityEventCallback[e] = calloc(sizeof(void (**)(unsigned int, void*)), maxEvents); //#TODO: MAX_EVENTS

		for (int i = 0; i < maxEvents; ++ i) {
			newWorld->entityEventCallback[e][i] = calloc(sizeof(void (*)(unsigned int, void*)), entityEventCallbackCountMax); //#TODO: MAX_ENTITY_EVENT_CALLBACKS
		}
	}

	newWorld->entityIdsToDelete = calloc(sizeof(unsigned int), newWorld->entityCountMax);
	newWorld->deletedEntityIds = calloc(sizeof(unsigned int), newWorld->entityCountMax);
	newWorld->systems = calloc(sizeof(void*), newWorld->entityCountMax);

	copyText(&newWorld->name, name);

	printf("[WORLD] Created new world: %s\n", name);
	
	addListItem(WORLD_LIST, newWorld);
}

void cleanWorld() {
	World *world = ACTIVE_WORLD;

	printf("[WORLD-CLEANING] Number of entities to clean: %i\n", world->entityIdsToDeleteCount);

	for (int i = 0; i < world->entityIdsToDeleteCount; ++ i) {
		unsigned int entityId = world->entityIdsToDelete[i];

		//#TODO: Clean up...
		
		-- world->entityIdsToDeleteCount;
		world->deletedEntityIds[world->deletedEntityCount] = entityId;
		++ world->deletedEntityCount;
	}
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
	for (listItem_t *item = WORLD_LIST->head; item; item = item->next) {
		World *world = item->item;
		
		if (!strcmp(world->name, name)) {
			ACTIVE_WORLD = world;

			printf("[WORLD] Active World changed to: %s\n", name);

			return;
		}
	}

	printf("[WORLD-#FATAL] Cannot get world: %s\n", name);

	assert(NULL);
}

void killWorld() {
	deleteListItem(WORLD_LIST, ACTIVE_WORLD);

	ACTIVE_WORLD = NULL;
}
