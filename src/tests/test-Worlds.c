#include <stdio.h>
#include "../framework/display.h"
#include "../world.h"
#include "../system.h"
#include "../entity.h"
#include "../component.h"
#include "../timestep.h"
#include "../sprite.h"
#include "../scene.h"
#include "components/health.h"
#include "components/worldPosition.h"
#include "constants.h"
#include "actors.h"


int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");
	
	initTimestep();
	initComponentHealth();
	initComponentWorldPosition();
	initComponentSprite();
	//initDisplay();

	unsigned int entityId = createEntity();
	unsigned int targetId = createHunter(0, 0, 0);
	
	registerHealth(entityId);

	DamageEvent tC = {entityId, targetId, 10};
	SceneComponent *sceneData = TEST_SCENE;

	triggerEvent(entityId, EVENT_HIT, &tC);
	stepTime();
	triggerEvents(EVENT_DRAW, 0, &sceneData);
	killWorld();
	destroyWorlds();
	destroyDisplay();
}
