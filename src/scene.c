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

SceneComponent *ACTIVE_SCENE = NULL;
linkedList_t *SCENES = NULL;


void deleteScene(void *data) {
	SceneComponent *scene = (SceneComponent*)data;

	free(scene->entityIds);
	free(scene->name);
}

void initScene() {
	SCENES = createLinkedList(&deleteScene);

	addComponentToWorld(&COMPONENT_SCENE, sizeof(SceneComponent));
	createSystem(EVENT_TIMESTEP_END, COMPONENT_SCENE, drawScene);
}

//#TODO: In the future, these should be entities.
void createScene(char *name, int size, int renderIndex) {
	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_SCENE);

	SceneComponent *scene = &getComponent(entityId, COMPONENT_SCENE)->scene;

	copyText(&scene->name, name);

	//#TODO: EntityIds never get free'd!
	scene->renderIndex = renderIndex;
	scene->entityIds = calloc(size, sizeof(unsigned int));

	scene->entityCount = 0;
	scene->entityCountMax = size;

	addListItem(SCENES, scene);

	printf("[SCENE] Created new scene: %s\n", scene->name);
}

void destroyScene() {
	deleteLinkedList(SCENES);
}

void setScene(char *name) {
	ACTIVE_SCENE = NULL;

	for (listItem_t *item = SCENES->head; item; item = item->next) {
		SceneComponent *scene = (SceneComponent*)item->item;
		printf("%s vs %s\n", name, scene->name);

		if (!strcmp(scene->name, name)) {
			ACTIVE_SCENE = scene;

			return;
		}
	}

	assert(ACTIVE_SCENE);
}

void getSceneName() {
	printf("Active Scene: %s\n", ACTIVE_SCENE->name);
}

void addEntityToScene(unsigned int entityId) {
	ACTIVE_SCENE->entityIds[ACTIVE_SCENE->entityCount] = entityId;

	++ ACTIVE_SCENE->entityCount;
}

void drawScene(unsigned int entityId, void *data) {
	//#TODO: Send camera data!
	Delta *timestepInfo = (Delta*)data;
	SceneComponent *scene = &getComponent(entityId, COMPONENT_SCENE)->scene;

	DrawEvent drawEvent;
	drawEvent.renderer = displayGetRenderer();
	drawEvent.delta = timestepInfo->delta;
	drawEvent.cameraZoom = 1.;
	drawEvent.cameraOffsetX = 0;
	drawEvent.cameraOffsetY = 0;

	for (int i = 0; i < scene->entityCount; ++ i)
		triggerEvent(scene->entityIds[i], EVENT_DRAW, &drawEvent);
}
