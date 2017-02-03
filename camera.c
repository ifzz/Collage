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
	camera->shakeRate = .8;
	camera->shakeX = 0;
	camera->shakeY = 0;
	camera->panX = 0;
	camera->panY = 0;
	camera->followingEntity = false;
	camera->viewportWidth = 0;
	camera->viewportHeight = 0;
	camera->panRate = .02;
	camera->zoomRate = .1;
	camera->enablePan = true;

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
	int minX = camera->x - 256;
	int minY = camera->y - 256;
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

	if (camera->enablePan) {
		camera->panExactX = interp(camera->panExactX, camera->nextPanExactX,
				camera->panRate);
		camera->panExactY = interp(camera->panExactY, camera->nextPanExactY,
				camera->panRate);
		camera->panX = round(camera->panExactX);
		camera->panY = round(camera->panExactY);
	} else {
		camera->panExactX = interp(camera->panExactX, 0, camera->panRate);
		camera->panExactY = interp(camera->panExactY, 0, camera->panRate);

		if (fabs(camera->panExactX) < 15 && fabs(camera->panExactY) < 15) {
			double weirdPanX = 3 * sin(getTimestepTicks() * .03);
			double weirdPanY = 3 * sin(getTimestepTicks() * .04);
			double maxPan = round(fabs(camera->panExactX));

			if (fabs(camera->panExactX) < fabs(camera->panExactY))
				maxPan = fabs(camera->panExactY);

			double mod = 1 - (maxPan / 15.);

			camera->panX = round(weirdPanX * mod) + 2;
			camera->panY = round(weirdPanY * mod) + 2;
		} else {
			camera->panX = round(camera->panExactX);
			camera->panY = round(camera->panExactY);
		}
	}

	camera->shakeX *= camera->shakeRate;
	camera->shakeY *= camera->shakeRate;

	camera->exactX += camera->panX;
	camera->exactY += camera->panY;

	double oldZoomWidth = camera->viewportWidth;
	double oldZoomHeight = camera->viewportHeight;

	int windowWidth = displayGetWindowWidth();
	int windowHeight = displayGetWindowHeight();
	int renderWidth = displayGetRenderWidth();
	int renderHeight = displayGetRenderHeight();

	double renderWidthScale;
	double renderHeightScale;

	if (windowWidth < renderWidth)
		renderWidthScale = renderWidth / (double)windowWidth;
	else
		renderWidthScale = windowWidth / (double)renderWidth;

	if (windowHeight < renderHeight)
		renderHeightScale = renderHeight / (double)windowHeight;
	else
		renderHeightScale = windowHeight / (double)renderHeight;

	double scaledZoomWidth = camera->zoom * renderWidthScale;
	double scaledZoomHeight = camera->zoom * renderHeightScale;

	camera->viewportWidth = round(displayGetWindowWidth() / scaledZoomWidth);
	camera->viewportHeight = round(displayGetWindowHeight() / scaledZoomHeight);

	camera->zoom = interp(camera->zoom, camera->nextZoom, camera->zoomRate);

	camera->nextExactX += (oldZoomWidth - camera->viewportWidth) / 2;
	camera->nextExactY += (oldZoomHeight - camera->viewportHeight) / 2;

	camera->x = round(camera->exactX + camera->shakeX);
	camera->y = round(camera->exactY + camera->shakeY);

	if (camera->followingEntity) {
		WorldPositionComponent *position =
			getComponent(camera->followingEntityId,
					COMPONENT_WORLD_POSITION);
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
	int windowWidth = displayGetWindowWidth();
	int windowHeight = displayGetWindowHeight();
	int renderWidth = displayGetRenderWidth();
	int renderHeight = displayGetRenderHeight();

	SDL_Rect displayRect = {0, 0, windowWidth, windowHeight};

	double renderWidthScale;
	double renderHeightScale;

	if (windowWidth < renderWidth)
		renderWidthScale = renderWidth / (double)windowWidth;
	else
		renderWidthScale = windowWidth / (double)renderWidth;

	if (windowHeight < renderHeight)
		renderHeightScale = renderHeight / (double)windowHeight;
	else
		renderHeightScale = windowHeight / (double)renderHeight;

	double scaledZoomWidth = camera->zoom * renderWidthScale;
	double scaledZoomHeight = camera->zoom * renderHeightScale;

	/*printf("%i - %i\n", renderWidth, windowWidth);*/

	SDL_RenderSetViewport(cameraInfo.renderer, &displayRect);
	SDL_RenderSetScale(cameraInfo.renderer, scaledZoomWidth, scaledZoomHeight);
	bool obeyingZoom = true;

	for (int z = 0; z <= stage->maxRenderIndex; ++ z) {
		for (int i = 0; i < camera->sceneCount; ++ i) {
			SceneComponent *scene = getScene(camera->stageName,
					camera->sceneNames[i]);

			if (scene->renderIndex != z)
				continue;

			if (scene->obeyZoom && !obeyingZoom) {
				SDL_RenderSetScale(cameraInfo.renderer, scaledZoomWidth,
						scaledZoomHeight);

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

void setCameraPanRate(unsigned int entityId, double rate) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->panRate = rate;
}

void setCameraZoomRate(unsigned int entityId, double rate) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->zoomRate = rate;
}

void setCameraShake(unsigned int entityId, double amount) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);
	double shake[2];

	velocity(shake, getRandomInt(360), amount);

	camera->shakeX = shake[0];
	camera->shakeY = shake[1];
}

void setCameraShakeRate(unsigned int entityId, double rate) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->shakeRate = rate;
}

void setCameraPanToggle(unsigned int entityId, bool on) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);

	camera->enablePan = on;
}
