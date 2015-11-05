#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "world.h"
#include "component.h"


void addComponentToWorld(unsigned int *id, size_t size) {
	World *world = getWorld();

	world->components[world->componentCount] = calloc(world->entityCountMax, sizeof(ComponentContainer));
	//world->components[world->componentCount] = malloc(world->entityCountMax * sizeof(ComponentContainer));

	assert(world->components[world->componentCount]);
	
	*id = 1 << world->componentCount;
	++ world->componentCount;
	
	printf("[WORLD] Added new component \'%u\' of size %lu\n", *id, sizeof(world->components[world->componentCount]));
}

void removeComponentFromWorld(unsigned int componentId) {
	World *world = getWorld();

	free(world->components[componentId]);

	printf("[WORLD] Removed component \'%u\' from world.\n", componentId);
}

void addComponentToEntity(unsigned int entityId, unsigned int componentFlag) {
	getWorld()->entityMask[entityId] |= componentFlag;

	printf("[ENTITY] Added entity #%u to component ID=%u\n", entityId, componentFlag);
}

ComponentContainer* getComponent(unsigned int entityId, unsigned int componentId) {
	int id = (int)log(componentId) / log(2);

	assert(entityId < 3000);
	//printf("Getting COMP id=%i for entity=%u\n", componentId, entityId);

	return &getWorld()->components[id][entityId];
}	
