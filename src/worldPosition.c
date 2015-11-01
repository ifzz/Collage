#include <stdio.h>
#include "worldPosition.h"
#include "system.h"
#include "component.h"

void eventSetWorldPositionCallback(unsigned int, void*);
//void eventDamageCallback(void*);


void initComponentWorldPosition() {
	addComponentToWorld(&COMPONENT_WORLD_POSITION, sizeof(WorldPositionComponent));
	createEvent(&EVENT_SET_POSITION);

	createSystem(EVENT_SET_POSITION, COMPONENT_WORLD_POSITION, eventSetWorldPositionCallback);
	//createSystem(EVENT_DAMAGE, COMPONENT_STATS, eventDamageCallback);
}

void registerWorldPosition(unsigned int entityId, int x, int y) {
	addComponentToEntity(entityId, COMPONENT_WORLD_POSITION);
	WorldPositionComponent *position = getComponent(entityId, COMPONENT_WORLD_POSITION);

	position->x = x;
	position->y = y;
}

void eventSetWorldPositionCallback(unsigned int entityId, void *data) {
	SetPositionEvent *newPosition = (SetPositionEvent*)data;
	WorldPositionComponent *position = getComponent(entityId, COMPONENT_WORLD_POSITION);

	position->x = newPosition->x;
	position->y = newPosition->y;

	printf("Set WORLD_POS: %i, %i\n", position->x, position->y);
}
