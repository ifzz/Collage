#define MAX_CAMERA_NAME_LEN 12

#include <stdbool.h>
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
	double panExactX, panExactY, nextPanExactX, nextPanExactY;
	double shakeX, shakeY;
	double zoom, nextZoom;
	double shakeRate;
	double panRate;
	double zoomRate;
	int viewportWidth, viewportHeight;
	int windowWidth, windowHeight;
	int sceneCount;
	int x, y;
	int panX, panY;
	unsigned int followingEntityId;
	bool enablePan;
	bool followingEntity;
	SDL_Texture *targetTexture;
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
void cameraUnfollowEntityId(unsigned int);
void setCameraLead(unsigned int, int, int);
void setCameraPanRate(unsigned int, double);
void setCameraSize(unsigned int, int, int);
void setCameraZoomRate(unsigned int, double);
void setCameraShake(unsigned int, double);
void setCameraShakeRate(unsigned int, double);
void setCameraPanToggle(unsigned int, bool);
unsigned int getCameraWithName(char*);
bool isInCameraViewport(CameraComponent*, int, int);
