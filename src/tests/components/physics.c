#include <stdio.h>
#include <math.h>
#include "physics.h"
#include "../constants.h"
#include "../../system.h"
#include "../../component.h"
#include "../../timestep.h"
#include "../../worldPosition.h"

typedef struct {
	float x;
	float v;
} State;

typedef struct {
	float dx;
	float dv;
} Derivative;

void eventSimulateCallback(unsigned int, void*);
void integrate(State*, float, float, float (*)(const State, float));


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
	physics->velocityX = 1.;
	physics->velocityY = 1.09;
}

float evaluateXVelocity(const State state, float t) {
	return 0;
}

float evaluateYVelocity(const State state, float t) {
	return 2;
}

void eventSimulateCallback(unsigned int entityId, void *data) {
	Delta *timestepInfo = (Delta*)data;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	State x = {physics->exactX, physics->velocityX};
	State y = {physics->exactY, physics->velocityY};

	integrate(&x, timestepInfo->time, timestepInfo->delta, &evaluateXVelocity);
	integrate(&y, timestepInfo->time, timestepInfo->delta, &evaluateYVelocity);

	physics->exactX = x.x;
	physics->velocityX = x.v;
	physics->exactY = y.x;
	physics->velocityY = y.v;

	SetPositionEvent positionData = {round(physics->exactX), round(physics->exactY)};
	
	triggerEvent(entityId, EVENT_SET_POSITION, &positionData);
} 


Derivative evaluate(const State initial, float t, float dt, const Derivative d, float (*callback)(const State, float)) {
	State state;
	Derivative output;

	state.x = initial.x + d.dx * dt;
	state.v = initial.v + d.dv * dt;

	output.dx = state.v;
	output.dv = callback(state, t + dt);
	return output;
}

Derivative evaluateInitial(const State initial, float t, float (*callback)(const State, float)) {
	Derivative output;

	output.dx = initial.v;
	output.dv = callback(initial, t);

	return output;

}	

void integrate(State *state, float t, float dt, float (*callback)(const State, float)) {
	Derivative a,b,c,d;

	a = evaluateInitial(*state, t, callback);
	b = evaluate(*state, t, dt * 0.5f, a, callback);
	c = evaluate(*state, t, dt * 0.5f, b, callback);
	d = evaluate(*state, t, dt, c, callback);

	float dxdt = 1.0f / 6.0f * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
	float dvdt = 1.0f / 6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

	state->x += dxdt * dt;
	state->v += dvdt * dt;
}
