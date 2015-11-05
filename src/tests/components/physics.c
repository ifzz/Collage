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

	createSystem(EVENT_TIMESTEP, COMPONENT_PHYSICS, eventSimulateCallback);
}

void registerPhysics(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_PHYSICS);
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	physics->exactX = position->x;
	physics->exactY = position->y;
	physics->velocityY = .01;
}

void eventSimulateCallback(unsigned int entityId, void *data) {
	Delta *timestepInfo = (Delta*)data;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	physics->velocityY += .05;

	physics->exactX += physics->velocityX;
	physics->exactY += physics->velocityY;

	SetPositionEvent positionData = {round(physics->exactX), round(physics->exactY)};
	
	triggerEvent(entityId, EVENT_SET_POSITION, &positionData);
}
