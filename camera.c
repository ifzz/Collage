#include "framework/display.h"
#include "component.h"
#include "timestep.h"
#include "system.h"
#include "camera.h"
#include "scene.h"


void renderCamera(unsigned int, void *data);


void initCameras() {
	addComponentToWorld(&COMPONENT_CAMERA, sizeof(CameraComponent));

	createSystem(EVENT_TIMESTEP_RENDER, COMPONENT_CAMERA, renderCamera);
}

void registerCamera(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_CAMERA);
}

void renderCamera(unsigned int entityId, void *data) {
	CameraComponent *camera = getComponent(entityId, COMPONENT_CAMERA);
	StageComponent *stage = getStage(camera->stageName);

	CameraRenderEvent cameraInfo;
	cameraInfo.entityId = entityId;
	cameraInfo.camera = camera;
	cameraInfo.delta = (Delta*)data;
	cameraInfo.renderer = displayGetRenderer();

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
