#include <stdio.h>
#include "../framework/display.h"
#include "../world.h"
#include "../system.h"
#include "../entity.h"
#include "../component.h"
#include "../timestep.h"
#include "../sprite.h"
#include "../scene.h"
#include "../worldPosition.h"
#include "components/health.h"
#include "components/physics.h"
#include "constants.h"
#include "actors.h"


void loop(void) {
	stepTime();
	displayPresent();
	displayClear();
}

int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");
	
	initTimestep();
	initComponentHealth();
	initComponentWorldPosition();
	initComponentPhysics();
	initComponentSprite();
	initScene();
	initDisplay();
	
	createScene("action", 100, 0);
	setScene("action");

	unsigned int targetId = createHunter(100, 43, 0);


	for (int i = 0; i < 5; ++ i)
		loop();

	/*killWorld();*/
	destroyWorlds();
	//#TODO: This should be done by the ECS!
	//destroyScene();
	destroyComponentSprite();
	destroyDisplay();
	return 0;
}
