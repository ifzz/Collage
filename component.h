#include <stdlib.h>
#include "sprite.h"
#include "scene.h"
#include "input.h"
#include "worldPosition.h"
//#include "tests/components/physics.h"
//#include "tests/components/health.h"
//
#define MAX_COMPONENT_NAME_LEN 32

#ifndef COMPONENT_H
#define COMPONENT_H

//typedef union componentContainer {
	////SpriteComponent sprite;
	//////HealthComponent health;
	////WorldPositionComponent worldPosition;
	//////PhysicsComponent physics;
	////InputComponent input;
	////SceneComponent scene;
	////StageComponent stage;
//} ComponentContainer;  

#endif

void addComponentToWorld(char*, unsigned int*, size_t);
bool hasComponents(unsigned int, unsigned int);
void removeComponentFromWorld(unsigned int);
void addComponentToEntity(unsigned int, unsigned int);
void* getComponent(unsigned int, unsigned int);
void* getAllComponents(unsigned int);
void listAllEntityComponents(unsigned int);
