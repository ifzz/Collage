#include <stdlib.h>
#include "sprite.h"
#include "worldPosition.h"
#include "scene.h"
#include "tests/components/physics.h"
#include "tests/components/health.h"

#ifndef COMPONENT_H
#define COMPONENT_H

typedef union componentContainer {
	SpriteComponent sprite;
	HealthComponent health;
	WorldPositionComponent worldPosition;
	PhysicsComponent physics;
	SceneComponent scene;

} ComponentContainer;  

#endif

void addComponentToWorld(unsigned int*, size_t);
void removeComponentFromWorld(unsigned int);
void addComponentToEntity(unsigned int, unsigned int);
ComponentContainer* getComponent(unsigned int, unsigned int);
