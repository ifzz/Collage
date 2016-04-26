#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "world.h"
#include "component.h"


int COMPONENT_SIZES[255];

void addComponentToWorld(unsigned int *id, size_t size) {
	World *world = getWorld();

	//NOTE: We can either init memory on the fly here, or do it at runtime.
	
	world->components[world->componentCount] = calloc(world->entityCountMax, size);

	assert(world->components[world->componentCount]);
	
	COMPONENT_SIZES[world->componentCount] = size;
	*id = 1 << world->componentCount;
	++ world->componentCount;
	
	/*printf("[WORLD] Added new component \'%u\' @ ID=%i of size %lu\n", *id, world->componentCount - 1, size);*/
}

void removeComponentFromWorld(unsigned int componentId) {
	World *world = getWorld();

	free(world->components[componentId]);

	printf("[WORLD] Removed component \'%u\' from world.\n", componentId);
}

void addComponentToEntity(unsigned int entityId, unsigned int componentFlag) {
	getWorld()->entityMask[entityId] |= componentFlag;

	/*printf("[ENTITY] Added entity #%u to component ID=%u\n", entityId, componentFlag);*/
}

void* getComponent(unsigned int entityId, unsigned int componentId) {
	World *world = getWorld();

	assert((world->entityMask[entityId] & componentId) == componentId && "Trying to access component not registered to entity");

	int id = (int)round(log(componentId) / log(2));

	return &world->components[id][entityId * COMPONENT_SIZES[id]];
}	

void* getAllComponents(unsigned int componentId) {
	int id = (int)round(log(componentId) / log(2));

	printf("STUB, test this first!\n");

	return getWorld()->components[id];
}

