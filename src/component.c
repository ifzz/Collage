#include <stdio.h>
#include "world.h"
#include "component.h"


void addComponentToWorld(unsigned int *id, size_t size) {
	World *world = getWorld();

	world->components[world->componentCount] = malloc(size * world->entityCountMax);
	
	*id = world->componentCount;//1 << world->componentCount;
	++ world->componentCount;
	
	printf("[WORLD] Added new component \'%u\' of size %zu\n", *id, size);
}

void* getComponent(unsigned int entityId, unsigned int componentId) {
	return &getWorld()->components[componentId][entityId];
}	
