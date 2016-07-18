#include "SDL2/SDL.h"
#include "timestep.h"
#include "scene.h"

unsigned int COMPONENT_CAMERA;

#ifndef CAMERA_H
#define CAMERA_H

typedef struct CameraComponent CameraComponent;
typedef struct CameraRenderEvent CameraRenderEvent;

struct CameraComponent {
	char stageName[MAX_STAGE_NAME_LEN];
	char sceneNames[MAX_SCENE_COUNT_PER_CAMERA][MAX_SCENE_NAME_LEN];
	double exactX, exactY, nextExactX, nextExactY;
	int sceneCount;
	int x, y;
};

struct CameraRenderEvent {
	Delta *delta;
	CameraComponent *camera;
	SDL_Renderer *renderer;
	unsigned int entityId;
};

#endif


void initCameras(void);
void registerCamera(unsigned int);
void assignCameraToStage(unsigned int, char*);
void addSceneToCamera(unsigned int, char*);
