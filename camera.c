#include <assert.h>
#include "worldPosition.h"
#include "framework/display.h"
#include "framework/numbers.h"
#include "component.h"
#include "timestep.h"
#include "system.h"
#include "camera.h"
#include "scene.h"


void tickCamera(unsigned int, void *data);
void renderCamera(unsigned int, void *data);
unsigned int FOLLOWING_ENTITY_ID = 0;
bool FOLLOWING_ENTITY = false;
unsigned int CAMERA_IDS[24];
int CAMERA_COUNT = 0;


void initCameras() {
	addComponentToWorld("camera", &COMPONENT_CAMERA, sizeof(CameraComponent));

	createSystem(EVENT_TIMESTEP_RENDER, COMPONENT_CAMERA, renderCamera);
	createSystem(EVENT_TICK, COMPONENT_CAMERA, tickCamera);
}

void registerCamera(unsigned int entityId, char *name) {
	addComponentToEntity(entityId, COMPONENT_CAMERA);

	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->zoom = 1.;
	camera->nextZoom = .9;
	camera->exactX = 0;
	camera->exactY = 0;
	camera->nextExactX = 0;
	camera->nextExactY = 0;
	camera->x = 0;
	camera->y = 0;
	camera->panExactX = 0;
	camera->panExactY = 0;
	camera->nextPanExactX = 0;
	camera->nextPanExactY = 0;
	camera->panX = 0;
	camera->panY = 0;
	camera->followingEntity = false;
	camera->viewportWidth = 0;
	camera->viewportHeight = 0;

	snprintf(camera->name, MAX_CAMERA_NAME_LEN, "%s", name);

	CAMERA_IDS[CAMERA_COUNT] = entityId;

	++ CAMERA_COUNT;
}

unsigned int getCameraWithName(char *name) {
	for (int i = 0; i < CAMERA_COUNT; ++ i) {
		CameraComponent *camera = getComponent(CAMERA_IDS[i],
				COMPONENT_CAMERA);

		if (!strcmp(camera->name, name))
			return CAMERA_IDS[i];
	}

	assert(1 == 2);
}

bool isInCameraViewport(CameraComponent *camera, int x, int y) {
	int minX = camera->x - 128;
	int minY = camera->y - 128;
	int maxX = camera->x + displayGetWindowWidth() / camera->zoom;
	int maxY = camera->y + displayGetWindowHeight() / camera->zoom;

	if (x < minX || x > maxX || y < minY || y > maxY)
		return false;

	return true;
}

void tickCamera(unsigned int entityId, void *data) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->exactX = interp(camera->exactX, camera->nextExactX, .1);
	camera->exactY = interp(camera->exactY, camera->nextExactY, .1);

	camera->panExactX = interp(camera->panExactX, camera->nextPanExactX, .02);
	camera->panExactY = interp(camera->panExactY, camera->nextPanExactY, .02);

	camera->panX = round(camera->panExactX);
	camera->panY = round(camera->panExactY);

	camera->exactX += camera->panX;
	camera->exactY += camera->panY;

	double oldZoomWidth = camera->viewportWidth;
	double oldZoomHeight = camera->viewportHeight;

	camera->viewportWidth = round(displayGetWindowWidth() / camera->zoom);
	camera->viewportHeight = round(displayGetWindowHeight() / camera->zoom);

	camera->zoom = interp(camera->zoom, camera->nextZoom, .1);

	camera->nextExactX += (oldZoomWidth - camera->viewportWidth) / 2;
	camera->nextExactY += (oldZoomHeight - camera->viewportHeight) / 2;

	camera->x = round(camera->exactX);
	camera->y = round(camera->exactY);

	if (camera->followingEntity) {
		WorldPositionComponent *position =
			getComponent(camera->followingEntityId, COMPONENT_WORLD_POSITION);
		camera->nextExactX = position->x - camera->viewportWidth / 2;
		camera->nextExactY = position->y - camera->viewportHeight / 2;
	}
}

void renderCamera(unsigned int entityId, void *data) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);
	StageComponent *stage = getStage(camera->stageName);

	CameraRenderEvent cameraInfo;
	cameraInfo.entityId = entityId;
	cameraInfo.camera = camera;
	cameraInfo.delta = (Delta*)data;
	cameraInfo.renderer = displayGetRenderer();

	SDL_Rect displayRect = {0, 0, displayGetWindowWidth(),
		displayGetWindowHeight()};

	SDL_RenderSetViewport(cameraInfo.renderer, &displayRect);
	SDL_RenderSetScale(cameraInfo.renderer, camera->zoom, camera->zoom);
	bool obeyingZoom = true;

	for (int z = 0; z <= stage->maxRenderIndex; ++ z) {
		for (int i = 0; i < camera->sceneCount; ++ i) {
			SceneComponent *scene = getScene(camera->stageName,
					camera->sceneNames[i]);

			if (scene->renderIndex != z)
				continue;

			if (scene->obeyZoom && !obeyingZoom) {
				SDL_RenderSetScale(cameraInfo.renderer, camera->zoom,
						camera->zoom);

				obeyingZoom = true;
			} else if (!scene->obeyZoom && obeyingZoom) {
				SDL_RenderSetScale(cameraInfo.renderer, 1, 1);

				obeyingZoom = false;
			}

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

void cameraFollowEntityId(unsigned int entityId, unsigned int targetId) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->followingEntity = true;
	camera->followingEntityId = targetId;
}

void cameraUnfollowEntityId(unsigned int entityId) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->followingEntity = false;
	camera->followingEntityId = 0;
}

void setCameraLead(unsigned int entityId, int x, int y) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->nextPanExactX = x;
	camera->nextPanExactY = y;
}
