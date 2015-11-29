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
#include "components/physics.h"
#include "components/health.h"
#include "constants.h"
#include "actors.h"
#include "ui.h"


void loop(void) {
	resetTimestep();

	while (!HARD_EXIT) {
		inputLoop();
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
	
	createStage("main");
	createScene("main", "action", 100, 0);
	createScene("main", "ui", 100, 1);

	createUi();
	
	createPlayer(100, 32, 1);

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
