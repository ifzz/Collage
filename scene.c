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

linkedList_t *STAGES = NULL;
StageComponent *ACTIVE_STAGE = NULL;

void deleteScene(void *data) {
	SceneComponent *scene = (SceneComponent*)data;

	free(scene->entityIds);
	free(scene->name);
}

void initScene() {
	STAGES = createLinkedList(&deleteScene);

	addComponentToWorld(&COMPONENT_SCENE, sizeof(SceneComponent));
	addComponentToWorld(&COMPONENT_STAGE, sizeof(StageComponent));
	createSystem(EVENT_TIMESTEP_RENDER, COMPONENT_STAGE, drawStage);
}

StageComponent* getStage(char *name) {
	for (listItem_t *stageItem = STAGES->head; stageItem; stageItem = stageItem->next) {
		StageComponent *stage = (StageComponent*)stageItem->item;

		if (!strcmp(stage->name, name))
			return stage;
	}

	printf("[STAGE-#FATAL] Could not find stage: %s\n", name);
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

	printf("[SCENE-#FATAL] Could not find scene: %s\n", sceneName);
	assert(1 == 2);

	return NULL;
}

void createStage(char *name) {
	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_STAGE);

	StageComponent *stage = getComponent(entityId, COMPONENT_STAGE);

	stage->scenes = createLinkedList(&deleteScene);
	stage->maxRenderIndex = 0;

	copyText(&stage->name, name);

	addListItem(STAGES, stage);

	printf("[STAGE] Created new stage: %s\n", stage->name);
}

void createScene(char *stageName, char *name, int size, int renderIndex) {
	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_SCENE);

	SceneComponent *scene = getComponent(entityId, COMPONENT_SCENE);

	copyText(&scene->name, name);

	//#TODO: EntityIds never get free'd!
	scene->renderIndex = renderIndex;
	scene->entityIds = calloc(size, sizeof(unsigned int));

	scene->entityCount = 0;
	scene->entityCountMax = size;

	printf("[SCENE] Created new scene: %s\n", scene->name);

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

				printf("Removed entity %i from scene %s\n", entityId,
						scene->name);
			}
		}
	}
}

void addEntityToScene(char *stageName, char *sceneName, unsigned int entityId) {
	SceneComponent *scene = getScene(stageName, sceneName);

	registerEntityEvent(entityId, EVENT_DELETE, &removeEntityFromSceneHandler);

	assert(scene->entityCount < scene->entityCountMax);

	scene->entityIds[scene->entityCount] = entityId;
	++ scene->entityCount;
}

void clearScene(char *stageName, char *sceneName) {
	SceneComponent *scene = getScene(stageName, sceneName);

	for (int i = 0; i < scene->entityCount; ++ i)
		deleteEntity(scene->entityIds[i]);
}

void drawScene(SceneComponent *scene, Delta *timestepInfo) {
	//#TODO: Send camera data!

	DrawEvent drawEvent;
	drawEvent.renderer = displayGetRenderer();
	drawEvent.delta = timestepInfo->delta;
	drawEvent.cameraZoom = 1.;
	drawEvent.cameraOffsetX = 0;
	drawEvent.cameraOffsetY = 0;

	for (int i = 0; i < scene->entityCount; ++ i)
		triggerEvent(scene->entityIds[i], EVENT_DRAW, &drawEvent);
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

