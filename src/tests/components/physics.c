#include <stdio.h>
#include <math.h>
#include "physics.h"
#include "../constants.h"
#include "../../system.h"
#include "../../component.h"
#include "../../timestep.h"
#include "../../worldPosition.h"

void eventSimulateCallback(unsigned int, void*);


void initComponentPhysics() {
	addComponentToWorld(&COMPONENT_PHYSICS, sizeof(PhysicsComponent));

	printf("PHYSICS=%u\n", COMPONENT_PHYSICS);
	//createSystem(EVENT_TIMESTEP, COMPONENT_PHYSICS, eventSimulateCallback);
}

void registerPhysics(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_PHYSICS);
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

}

void eventSimulateCallback(unsigned int entityId, void *data) {
	Delta *timestepInfo = (Delta*)data;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	physics->exactX += physics->velocityX;
	physics->exactY += physics->velocityY;

	SetPositionEvent positionData = {round(physics->exactX), round(physics->exactY)};

	triggerEvent(entityId, EVENT_SET_POSITION, &positionData);
}
