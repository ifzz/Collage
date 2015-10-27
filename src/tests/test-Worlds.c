#include <stdio.h>
#include "../framework/display.h"
#include "../world.h"
#include "../system.h"
#include "../entity.h"
#include "../component.h"
#include "../timestep.h"
#include "components/health.h"
#include "constants.h"
#include "actors.h"


int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");
	
	initTimestep();
	initComponentHealth();
	initDisplay();

	unsigned int entityId = createEntity();
	unsigned int targetId = createHunter(0, 0, 0);
	
	registerHealth(entityId);

	DamageEvent tC = {entityId, targetId, 10};

	triggerEvent(entityId, EVENT_HIT, &tC);
	stepTime();
	killWorld();
	destroyWorlds();
	destroyDisplay();
}
