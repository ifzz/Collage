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
#include "../input.h"
#include "components/health.h"
#include "components/physics.h"
#include "constants.h"
#include "actors.h"


void loop(void) {
	resetTimestep();

	while (SDL_GetTicks() < 3200) {
		stepTime();
		displayPresent();
		displayClear();
	}
}

int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");
	
	initTimestep();
	initInput();
	initComponentHealth();
	initComponentWorldPosition();
	initComponentPhysics();
	initComponentSprite();
	initScene();
	initDisplay();
	
	createScene("action", 100, 0);
	setScene("action");

	createActor(100, 32, 1);

	loop();

	//killWorld();
	destroyWorlds();

	//#TODO: This should be done by the ECS!
	//destroyScene();
	destroyComponentSprite();
	destroyDisplay();
	showTimestepInfo();

	return 0;
}
