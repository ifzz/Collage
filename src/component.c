#include <stdio.h>
#include <stdlib.h>
#include "world.h"
#include "component.h"


void addComponentToWorld(unsigned int *id, size_t size) {
	World *world = getWorld();

	world->components[world->componentCount] = malloc(size * world->entityCountMax);
	
	*id = world->componentCount;//1 << world->componentCount;
	++ world->componentCount;
	
	printf("[WORLD] Added new component \'%u\' of size %zu\n", *id, size);
}

void removeComponentFromWorld(unsigned int id) {
	World *world = getWorld();

	free(world->components[id]);

	printf("[WORLD] Removed component \'%u\' from world.\n", id);
}

void addComponentToEntity(unsigned int entityId, unsigned int componentFlag) {
	getWorld()->entityMask[entityId] |= componentFlag;

	printf("[ENTITY] Added entity #%u to component ID=%u\n", entityId, componentFlag);
}

void* getComponent(unsigned int entityId, unsigned int componentId) {
	return &getWorld()->components[componentId][entityId];
}	
