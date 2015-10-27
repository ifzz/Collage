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


void initWorlds() {
	WORLD_LIST = createLinkedList(&deleteWorld);
}

void destroyWorlds() {
	deleteLinkedList(WORLD_LIST);
}

void createWorld(char *name) {
	World *newWorld = calloc(sizeof(World), 1);

	//#TODO: This should be MAX_EVENTS.
	int entityEventCallbackCountMax = 300;

	newWorld->entityCount = 0;
	newWorld->entityCountMax = 3000;
	newWorld->systemCountMax = 100;
	newWorld->entityMask = calloc(sizeof(unsigned int), newWorld->entityCountMax);
	newWorld->components = calloc(sizeof(void*), newWorld->entityCountMax);
	newWorld->entityEventCallbackCount = calloc(sizeof(unsigned int*), newWorld->entityCountMax);
	
	for (int e = 0; e < newWorld->entityCountMax; ++ e)
		newWorld->entityEventCallbackCount[e] = calloc(sizeof(int), entityEventCallbackCountMax);
	
	newWorld->systemIndex = calloc(sizeof(unsigned int), newWorld->entityCountMax);
	newWorld->systemMask = calloc(sizeof(unsigned int*), newWorld->systemCountMax);

	for (int i = 0; i < newWorld->systemCountMax; ++ i)
		newWorld->systemMask[i] = calloc(sizeof(unsigned int), newWorld->systemCountMax);

	newWorld->systemCallback = calloc(sizeof(void (*)(void*)), newWorld->systemCountMax);
	
	for (int i = 0; i < newWorld->systemCountMax; ++ i)
		newWorld->systemCallback[i] = calloc(sizeof(void (*)(void*)), newWorld->systemCountMax);
	
	newWorld->entityEventCallback = calloc(sizeof(void (*)(void*)), newWorld->entityCountMax);

	for (int e = 0; e < newWorld->entityCountMax; ++ e) {
		newWorld->entityEventCallback[e] = calloc(sizeof(void (*)(void*)), entityEventCallbackCountMax);

		for (int i = 0; i < entityEventCallbackCountMax; ++ i) {
			newWorld->entityEventCallback[e][i] = calloc(sizeof(void (*)(void*)), entityEventCallbackCountMax);
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
	
	for (int i = 0; i < world->componentCount; ++ i)
		removeComponentFromWorld(i);

	for (int i = 0; i < world->systemCountMax; ++ i)
		free(world->systemMask[i]);

	for (int i = 0; i < world->systemCountMax; ++ i)
		free(world->systemCallback[i]);
	
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
