#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "framework/list.h"
#include "framework/strings.h"
#include "scene.h"
#include "system.h"
#include "sprite.h"

Scene *ACTIVE_SCENE = NULL;
linkedList_t *SCENES = NULL;

void deleteScene(void *data) {
	Scene *scene = data;

	free(scene->entityIds);
	free(scene->name);
}

void initScene() {
	SCENES = createLinkedList(&deleteScene);
}

//#TODO: In the future, these should be entities.
void createScene(char *name, int size, int renderIndex) {
	Scene *scene = calloc(sizeof(Scene), 1);

	copyText(&scene->name, name);

	scene->renderIndex = renderIndex;
	scene->entityIds = malloc(sizeof(unsigned int) * size);
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
		Scene *scene = item->item;

		if (!strcmp(scene->name, name)) {
			ACTIVE_SCENE = scene;

			return;
		}
	}

	assert(ACTIVE_SCENE);
}

void addEntityToScene(char *sceneName, unsigned int entityId) {
	ACTIVE_SCENE->entityIds[ACTIVE_SCENE->entityCount] = entityId;

	++ ACTIVE_SCENE->entityCount;
}

void drawScene() {
	//#TODO: Send camera data!
	//
	for (int i = 0; i < ACTIVE_SCENE->entityCount; ++ i)
		triggerEvent(ACTIVE_SCENE->entityIds[i], EVENT_DRAW, NULL);
}
