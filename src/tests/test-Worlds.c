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

double FRAMES = 0;
double FRAME_TIMER = 0.;
double FPS_CAP = -1; //1000 / 400.;


void loop(void) {
	while (SDL_GetTicks() < 3200) {
		stepTime();
		displayPresent();
		displayClear();

		++ FRAMES;

		if (FPS_CAP > -1) {
			double ticks = (SDL_GetTicks() - FRAME_TIMER) / 1000.;

			if (ticks < FPS_CAP) {
				SDL_Delay(FPS_CAP - ticks);
			}
		}

		if (SDL_GetTicks() - FRAME_TIMER >= 1000.) {
			printf("FPS=%f\n", FRAMES);

			FRAMES = 0;
			FRAME_TIMER = SDL_GetTicks();
		}
	}
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
	
	createScene("action", 1000, 0);
	setScene("action");

	for (int i = 0; i < 1000; ++ i)
		createHunter(100 + 16 * i, 32, 0);

	FRAME_TIMER = SDL_GetTicks();
	loop();

	/*killWorld();*/
	destroyWorlds();
	//#TODO: This should be done by the ECS!
	//destroyScene();
	destroyComponentSprite();
	destroyDisplay();
	showTimestepInfo();
	return 0;
}
