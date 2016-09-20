#include <stdbool.h>
#include "framework/list.h"

unsigned int COMPONENT_STAGE;
unsigned int COMPONENT_SCENE;

#define MAX_STAGE_NAME_LEN 25
#define MAX_SCENE_NAME_LEN 25
#define MAX_SCENE_COUNT_PER_CAMERA 10

#ifndef SCENE_H
#define SCENE_H

typedef struct {
	char name[MAX_STAGE_NAME_LEN];
	int renderIndex;
	int entityCount;
	int entityCountMax;
	unsigned int *entityIds;
	bool obeyZoom;
} SceneComponent;

typedef struct {
	char name[MAX_SCENE_NAME_LEN];
	linkedList_t *scenes;
	int maxRenderIndex;
} StageComponent;

#endif


void initScene(void);
void destroyScene(void);
void createStage(char*);
void createScene(char*, char*, int, int, bool);
void drawScene(SceneComponent*, void*);
StageComponent* getStage(char*);
SceneComponent* getScene(char*, char*);
void clearScene(char*, char*);
void addEntityToScene(char *, char *, unsigned int);
void drawStage(unsigned int, void*);
