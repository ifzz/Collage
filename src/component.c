#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "world.h"
#include "component.h"


void addComponentToWorld(unsigned int *id, size_t size) {
	World *world = getWorld();

	//NOTE: We can either init memory on the fly here, or do it at runtime.
	
	/*world->components[world->componentCount] = calloc(world->entityCountMax, sizeof(ComponentContainer));*/
	/*world->components[world->componentCount] = malloc(world->entityCountMax * sizeof(ComponentContainer));*/

	assert(world->components[world->componentCount]);
	
	*id = 1 << world->componentCount;
	++ world->componentCount;
	
	printf("[WORLD] Added new component \'%u\' @ ID=%i of size %lu\n", *id, world->componentCount - 1, sizeof(ComponentContainer));
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
	int id = (int)round(log(componentId) / log(2));

	/*printf("Getting COMP %i with ID=%i for entity=%u\n", componentId, id, entityId);*/

	return &getWorld()->components[id][entityId];
}	

ComponentContainer* getAllComponents(unsigned int componentId) {
	int id = (int)round(log(componentId) / log(2));

	printf("STUB, test this first!\n");

	return getWorld()->components[id];
}

