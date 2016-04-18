#include <stdio.h>
#include <assert.h>
#include "worldPosition.h"
#include "timestep.h"
#include "system.h"
#include "component.h"

void eventSetWorldPositionCallback(unsigned int, void*);
void eventUpdateWorldPosition(unsigned int, void*);
//void eventDamageCallback(void*);


void initComponentWorldPosition() {
	addComponentToWorld(&COMPONENT_WORLD_POSITION, sizeof(WorldPositionComponent));
	createEvent(&EVENT_SET_POSITION);

	createSystem(EVENT_TIMESTEP, COMPONENT_WORLD_POSITION, eventUpdateWorldPosition);
	createSystem(EVENT_SET_POSITION, COMPONENT_WORLD_POSITION, eventSetWorldPositionCallback);
}

void registerWorldPosition(unsigned int entityId, int x, int y) {
	addComponentToEntity(entityId, COMPONENT_WORLD_POSITION);
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	position->x = x;
	position->y = y;
}

void eventUpdateWorldPosition(unsigned int entityId, void *data) {
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	position->lastX = position->x;
	position->lastY = position->y;
}

void eventSetWorldPositionCallback(unsigned int entityId, void *data) {
	SetPositionEvent *newPosition = (SetPositionEvent*)data;
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	position->lastX = position->x;
	position->lastY = position->y;
	position->x = newPosition->x;
	position->y = newPosition->y;
}
