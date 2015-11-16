#include <stdio.h>
#include <math.h>
#include "physics.h"
#include "../constants.h"
#include "../../system.h"
#include "../../component.h"
#include "../../timestep.h"
#include "../../worldPosition.h"

void eventSimulateCallback(unsigned int, void*);
void integrate(PhysicsComponent*, float, float);

typedef struct {
	float x;
	float v;
} State;

typedef struct {
	float dx;
	float dv;
} Derivative;

void initComponentPhysics() {
	addComponentToWorld(&COMPONENT_PHYSICS, sizeof(PhysicsComponent));

	createSystem(EVENT_TIMESTEP, COMPONENT_PHYSICS, eventSimulateCallback);
}

void registerPhysics(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_PHYSICS);
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	physics->lastExactX = position->x;
	physics->lastExactY = position->y;
	physics->exactX = position->x;
	physics->exactY = position->y;
	physics->velocityX = 3.;
	physics->velocityY = 3.09;
}

void eventSimulateCallback(unsigned int entityId, void *data) {
	Delta *timestepInfo = (Delta*)data;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	//double velX = physics->exactX - physics->lastExactX;
	//double velY = physics->exactY - physics->lastExactY;

	//physics->lastExactX = physics->exactX;
	//physics->lastExactY = physics->exactY;

	//physics->exactX += velX + 0.0;
	//
	//physics->exactY += physics->velocityY;
	integrate(physics, timestepInfo->time, timestepInfo->delta);

	SetPositionEvent positionData = {round(physics->exactX), round(physics->exactY)};
	
	triggerEvent(entityId, EVENT_SET_POSITION, &positionData);
} 

float acceleration(const PhysicsComponent state, float t) {
	const float k = .09; //Force
	const float b = 1;

	return k * state.exactX - b*state.velocityX;
}

Derivative evaluate(const PhysicsComponent initial, float t, float dt, const Derivative d) {
	PhysicsComponent state;
	Derivative output;

	state.exactX = initial.exactX + d.dx*dt; //Needs function for y?
	state.velocityX = initial.velocityX + d.dv*dt; //Needs function for vY?

	output.dx = state.velocityX; //Should this be exactX?
	output.dv = acceleration(state, t+dt);
	return output;
}

Derivative evaluateInitial(const PhysicsComponent initial, float t) {
	Derivative output;

	output.dx = initial.velocityX;
	output.dv = acceleration(initial, t);

	return output;

}	

void integrate(PhysicsComponent *state, float t, float dt) {
	Derivative a,b,c,d;

	a = evaluateInitial( *state, t);
	b = evaluate( *state, t, dt*0.5f, a );
	c = evaluate( *state, t, dt*0.5f, b );
	d = evaluate( *state, t, dt, c );

	float dxdt = 1.0f / 6.0f * 
		( a.dx + 2.0f*(b.dx + c.dx) + d.dx );

	float dvdt = 1.0f / 6.0f * 
		( a.dv + 2.0f*(b.dv + c.dv) + d.dv );

	//printf("TIME=%f, DT=%f\n", t, dt);
	//printf("\t%f (%f), %f (%f)\n", dxdt,  dxdt * dt, dvdt, dvdt * dt);
	//printf("BEFORE: %f\n", state->exactX);

	state->exactX = state->exactX + dxdt * dt;
	state->velocityX = state->velocityX + dvdt * dt;

	//printf("AFTER: %f\n", state->exactX);
}
