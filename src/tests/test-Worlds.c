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
#include "levels.h"
#include "actors.h"
#include "items.h"
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
	initItems();
	initScene();
	initDisplay();
	
	createStage("main");
	createScene("main", "action", 100, 0);
	createScene("main", "ui", 100, 1);
	createLevel(displayGetRenderWidth() / 16, displayGetRenderHeight() / 16);

	createUi();

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
