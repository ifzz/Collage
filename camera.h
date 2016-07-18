#include "scene.h"

unsigned int COMPONENT_CAMERA;

#ifndef CAMERA_H
#define CAMERA_H

typedef struct CameraComponent CameraComponent;

struct CameraComponent {
	char stageName[MAX_STAGE_NAME_LEN];
	char sceneNames[MAX_SCENE_COUNT_PER_CAMERA]
};

#endif
