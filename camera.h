#define MAX_CAMERA_NAME_LEN 12

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
	char name[MAX_CAMERA_NAME_LEN];
	double exactX, exactY, nextExactX, nextExactY;
	double zoom, nextZoom;
	int viewportWidth, viewportHeight;
	int sceneCount;
	int x, y;
	unsigned int followingEntityId;
	bool followingEntity;
};

struct CameraRenderEvent {
	Delta *delta;
	CameraComponent *camera;
	SDL_Renderer *renderer;
	unsigned int entityId;
};

#endif


void initCameras(void);
void registerCamera(unsigned int, char*);
void assignCameraToStage(unsigned int, char*);
void addSceneToCamera(unsigned int, char*);
void cameraFollowEntityId(unsigned int, unsigned int);
unsigned int getCameraWithName(char*);
