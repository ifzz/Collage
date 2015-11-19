#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct {
	float lastExactX; //Cut
	float lastExactY; //Cut
	float exactX;
	float exactY;
	float velocityX;
	float velocityY;
	float frictionX;
	float frictionY;
} PhysicsComponent;

#endif


void initComponentPhysics(void);
void registerPhysics(unsigned int);

