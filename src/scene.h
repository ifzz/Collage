#ifndef SCENE_H
#define SCENE_H

typedef struct {
	char *name;
	int renderIndex;
	int entityCount;
	int entityCountMax;
	unsigned int *entityIds;
} Scene;

#endif


void initScene(void);
void destroyScene(void);
void createScene(char*, int, int);
void setScene(char *name);
void addEntityToScene(char*, unsigned int);
void drawScene(void);
