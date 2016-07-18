#include "framework/display.h"
#include "framework/numbers.h"
#include "component.h"
#include "timestep.h"
#include "system.h"
#include "camera.h"
#include "scene.h"


void tickCamera(unsigned int, void *data);
void renderCamera(unsigned int, void *data);


void initCameras() {
	addComponentToWorld(&COMPONENT_CAMERA, sizeof(CameraComponent));

	createSystem(EVENT_TIMESTEP_RENDER, COMPONENT_CAMERA, renderCamera);
	createSystem(EVENT_TICK, COMPONENT_CAMERA, tickCamera);
}

void registerCamera(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_CAMERA);

	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->zoom = 1.;
	camera->nextZoom = 1.;
	camera->exactX = 100.;
	camera->exactY = 100.;
	camera->nextExactX = 150.;
	camera->nextExactY = 150.;
	camera->x = 100;
	camera->y = 100;
}

void tickCamera(unsigned int entityId, void *data) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->exactX = interp(camera->exactX, camera->nextExactX, .1);
	camera->exactY = interp(camera->exactY, camera->nextExactY, .1);

	camera->x = round(camera->exactX);
	camera->y = round(camera->exactY);

	camera->zoom = interp(camera->zoom, camera->nextZoom, .1);
}

void renderCamera(unsigned int entityId, void *data) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);
	StageComponent *stage = getStage(camera->stageName);

	CameraRenderEvent cameraInfo;
	cameraInfo.entityId = entityId;
	cameraInfo.camera = camera;
	cameraInfo.delta = (Delta*)data;
	cameraInfo.renderer = displayGetRenderer();

	SDL_RenderSetScale(cameraInfo.renderer, camera->zoom, camera->zoom);

	for (int z = 0; z <= stage->maxRenderIndex; ++ z) {
		for (int i = 0; i < camera->sceneCount; ++ i) {
			SceneComponent *scene = getScene(camera->stageName,
					camera->sceneNames[i]);

			if (scene->renderIndex != z)
				continue;

			drawScene(scene, &cameraInfo);
		}
	}
}

void assignCameraToStage(unsigned int entityId, char *stageName) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	snprintf(camera->stageName, MAX_STAGE_NAME_LEN, "%s", stageName);

	addEntityToScene(stageName, "_cameras", entityId);

	camera->sceneCount = 0;
}

void addSceneToCamera(unsigned int entityId, char *sceneName) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	snprintf(camera->sceneNames[camera->sceneCount], MAX_SCENE_NAME_LEN,
			"%s", sceneName);

	++ camera->sceneCount;
}
