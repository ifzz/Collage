#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "framework/list.h"
#include "framework/display.h"
#include "framework/strings.h"
#include "scene.h"
#include "timestep.h"
#include "system.h"
#include "sprite.h"
#include "entity.h"
#include "component.h"
#include "debug.h"

linkedList_t *STAGES = NULL;
StageComponent *ACTIVE_STAGE = NULL;

void startStage(unsigned int, void *data);
void tickStage(unsigned int, void *data);

void deleteScene(void *data) {
	SceneComponent *scene = (SceneComponent*)data;

	free(scene->entityIds);
}

void initScene() {
	STAGES = createLinkedList(&deleteScene);

	addComponentToWorld("scene", &COMPONENT_SCENE, sizeof(SceneComponent));
	addComponentToWorld("stage", &COMPONENT_STAGE, sizeof(StageComponent));
	createSystem(EVENT_TIMESTEP, COMPONENT_STAGE, tickStage);
	/*createSystem(EVENT_TIMESTEP_RENDER, COMPONENT_STAGE, drawStage);*/
	createSystem(EVENT_TIMESTEP_START, COMPONENT_STAGE, startStage);
}

StageComponent* getStage(char *name) {
	assert(STAGES != NULL);

	for (listItem_t *stageItem = STAGES->head; stageItem; stageItem = stageItem->next) {
		StageComponent *stage = (StageComponent*)stageItem->item;

		if (!strcmp(stage->name, name))
			return stage;
	}

	logError("[STAGE-#FATAL] Could not find stage: %s", name);
	assert(1 == 2);

	return NULL;
}

SceneComponent* getScene(char *stageName, char *sceneName) {
	StageComponent *stage = getStage(stageName);

	for (listItem_t *item = stage->scenes->head; item; item = item->next) {
		SceneComponent *scene = (SceneComponent*)item->item;

		if (!strcmp(scene->name, sceneName))
			return scene;
	}

	logError("[SCENE-#FATAL] Could not find scene: %s", sceneName);
	assert(1 == 2);

	return NULL;
}

void createStage(char *name) {
	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_STAGE);

	StageComponent *stage = getComponent(entityId, COMPONENT_STAGE);

	stage->scenes = createLinkedList(&deleteScene);
	stage->maxRenderIndex = 0;

	snprintf(stage->name, MAX_STAGE_NAME_LEN, "%s", name);

	addListItem(STAGES, stage);

	createScene(name, "_cameras", 25, -1, false);

	logInfo("[STAGE] Created new stage: %s", stage->name);
}

void createScene(char *stageName, char *name, int size, int renderIndex,
		bool obeyZoom) {
	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_SCENE);

	SceneComponent *scene = getComponent(entityId, COMPONENT_SCENE);

	snprintf(scene->name, MAX_SCENE_NAME_LEN, "%s", name);

	//#TODO: EntityIds never get free'd!
	scene->renderIndex = renderIndex;
	scene->entityIds = calloc(size, sizeof(unsigned int));

	scene->entityCount = 0;
	scene->entityCountMax = size;

	scene->obeyZoom = obeyZoom;

	logInfo("[SCENE] Created new scene: %s", scene->name);

	StageComponent *stage = getStage(stageName);

	if (!strcmp(stage->name, stageName)) {
		addListItem(stage->scenes, scene);

		if (scene->renderIndex > stage->maxRenderIndex)
			stage->maxRenderIndex = scene->renderIndex;

		return;
	}
}

void destroyScene() {
	deleteLinkedList(STAGES);
}

void removeEntityFromSceneHandler(unsigned int entityId, void *data) {
	for (listItem_t *stageItem = STAGES->head; stageItem; stageItem = stageItem->next) {
		StageComponent *stage = (StageComponent*)stageItem->item;

		for (listItem_t *item = stage->scenes->head; item; item = item->next) {
			SceneComponent *scene = (SceneComponent*)item->item;
			bool deleting = false;

			for (int i = 0; i < scene->entityCount; ++ i) {
				if (scene->entityIds[i] == entityId)
					deleting = true;

				if (deleting)
					scene->entityIds[i] = scene->entityIds[i + 1];
			}

			if (deleting) {
				-- scene->entityCount;

				assert(scene->entityCount >= 0);

				logInfo("Removed entity %i from scene %s", entityId,
						scene->name);
			}
		}
	}
}

void addEntityToScene(char *stageName, char *sceneName, unsigned int entityId) {
	SceneComponent *scene = getScene(stageName, sceneName);

	registerEntityEvent(entityId, EVENT_DELETE, &removeEntityFromSceneHandler);

	if (scene->entityCount >= scene->entityCountMax) {
		printf("Scene `%s` hit entity limit: %i\n", sceneName,
				scene->entityCountMax);
	}

	assert(scene->entityCount < scene->entityCountMax);

	scene->entityIds[scene->entityCount] = entityId;
	++ scene->entityCount;
}

void removeEntityFromScene(unsigned int entityId) {
	removeEntityFromSceneHandler(entityId, NULL);
}

void clearScene(char *stageName, char *sceneName) {
	SceneComponent *scene = getScene(stageName, sceneName);

	for (int i = 0; i < scene->entityCount; ++ i)
		deleteEntity(scene->entityIds[i]);
}

void tickScene(SceneComponent *scene, Delta *timestepInfo) {
	int timestepTicks = getTimestepTicks();

	for (int i = 0; i < scene->entityCount; ++ i) {
		if (timestepTicks % 2)
			triggerEvent(scene->entityIds[i], EVENT_HALF_TICK, timestepInfo);

		triggerEvent(scene->entityIds[i], EVENT_TICK, timestepInfo);
	}
}

void startScene(SceneComponent *scene, Delta *timestepInfo) {
	for (int i = 0; i < scene->entityCount; ++ i)
		triggerEvent(scene->entityIds[i], EVENT_TIMESTEP_START, timestepInfo);
}

void drawScene(SceneComponent *scene, void *data) {
	//#TODO: Send camera data!

	/*DrawEvent drawEvent;*/
	/*drawEvent.renderer = displayGetRenderer();*/
	/*drawEvent.delta = timestepInfo->delta;*/
	/*drawEvent.cameraZoom = 1.;*/
	/*drawEvent.cameraOffsetX = 0;*/
	/*drawEvent.cameraOffsetY = 0;*/
	
	for (int i = 0; i < scene->entityCount; ++ i)
		triggerEvent(scene->entityIds[i], EVENT_DRAW, data);
}

void tickStage(unsigned int entityId, void *data) {
	StageComponent *stage = getComponent(entityId, COMPONENT_STAGE);
	Delta *timestepInfo = (Delta*)data;

	for (listItem_t *item = stage->scenes->head; item; item = item->next) {
		SceneComponent *scene = (SceneComponent*)item->item;

		tickScene(scene, timestepInfo);
	}
}

void startStage(unsigned int entityId, void *data) {
	StageComponent *stage = getComponent(entityId, COMPONENT_STAGE);
	Delta *timestepInfo = (Delta*)data;

	for (listItem_t *item = stage->scenes->head; item; item = item->next) {
		SceneComponent *scene = (SceneComponent*)item->item;

		startScene(scene, timestepInfo);
	}
}

void drawStage(unsigned int entityId, void *data) {
	StageComponent *stage = getComponent(entityId, COMPONENT_STAGE);
	Delta *timestepInfo = (Delta*)data;

	for (int z = 0; z <= stage->maxRenderIndex; ++ z) {
		for (listItem_t *item = stage->scenes->head; item; item = item->next) {
			SceneComponent *scene = (SceneComponent*)item->item;

			if (scene->renderIndex != z)
				continue;

			drawScene(scene, timestepInfo);
		}
	}
}

