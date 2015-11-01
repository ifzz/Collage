#include <stdlib.h>
#include "framework/list.h"
#include "framework/strings.h"
#include "scene.h"

//#TODO: In the future, these should be entities.
linkedList_t *SCENES = NULL;

void deleteScene(void *data) {
	SceneComponent *scene = data;

	free(scene->name);
}

void initScene() {
	SCENES = createLinkedList(&deleteScene);
}

void createScene(char *name, int renderIndex) {
	SceneComponent *scene = calloc(sizeof(SceneComponent), 1);

	copyText(&scene->name, name);
	scene->renderIndex = renderIndex;
}

