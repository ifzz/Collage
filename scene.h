#include "framework/list.h"

unsigned int COMPONENT_STAGE;
unsigned int COMPONENT_SCENE;

#ifndef SCENE_H
#define SCENE_H

typedef struct {
	char *name;
	int renderIndex;
	int entityCount;
	int entityCountMax;
	unsigned int *entityIds;
} SceneComponent;

typedef struct {
	char *name;
	linkedList_t *scenes;
	int maxRenderIndex;
} StageComponent;

#endif


void initScene(void);
void destroyScene(void);
void createStage(char*);
void createScene(char*, char*, int, int);
void addEntityToScene(char *, unsigned int);
void drawStage(unsigned int, void*);
