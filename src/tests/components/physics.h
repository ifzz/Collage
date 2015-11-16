#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct {
	double lastExactX; //Cut
	double lastExactY; //Cut
	double exactX;
	double exactY;
	double velocityX;
	double velocityY;
} PhysicsComponent;

#endif


void initComponentPhysics(void);
void registerPhysics(unsigned int);

