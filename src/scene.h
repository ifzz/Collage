#ifndef SCENE_H
#define SCENE_H

typedef struct {
	char *name;
	int renderIndex;
	int entityCount;
	unsigned int *entityIds;
} SceneComponent;

#endif


void initScenes(void);
void createScene(char*, int);
