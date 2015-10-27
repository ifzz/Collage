#include <stdio.h>
#include "../framework/display.h"
#include "../world.h"
#include "../system.h"
#include "../entity.h"
#include "../component.h"
#include "componentHealth.h"
#include "../timestep.h"
#include "constants.h"


int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");
	
	initTimestep();
	initComponentHealth();
	initDisplay();


	unsigned int entityId = createEntity();
	unsigned int targetId = createEntity();

	registerHealth(entityId);
	registerHealth(targetId);

	DamageEvent tC = {entityId, targetId, 10};

	triggerEvent(entityId, EVENT_HIT, &tC);
	stepTime();
	//triggerEvents(COMPONENT_TEST_COMP, EVENT_TICK, NULL);
	killWorld();
	destroyWorlds();
	destroyDisplay();
}
