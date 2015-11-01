//#include <SDL2/SDL.h>

#ifndef SCENE_H
#define SCENE_H

typedef struct {
	//SDL_Renderer *rederer;
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
void addEntityToScene(unsigned int);
void drawScene(void);
