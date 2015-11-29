#include <stdio.h>
#include "../framework/display.h"
#include "../component.h"
#include "../entity.h"
#include "../sprite.h"
#include "../system.h"
#include "../input.h"
#include "../scene.h"
#include "../worldPosition.h"
#include "../timestep.h"
#include "components/health.h"
#include "components/physics.h"
#include "constants.h"
#include "abilities.h"
#include "actors.h"


void actorGridMoveHandler(unsigned int entityId, void *data) {
	InputComponent *input = &getComponent(entityId, COMPONENT_INPUT)->input;
	/*PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;*/
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	if (input->movingLeft && position->x > 16 * 1) {
		MovementEvent moveInfo = {-5, 0};

		triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
	} else if (input->movingRight && position->x < 16 * ((displayGetRenderWidth() / 16) - 1)) {
		MovementEvent moveInfo = {5, 0};

		triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
	} else if (!(position->x % 16)) {
		MovementEvent moveInfo = {0, 0};

		triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
	}
}

void actorKeyInputHandler(unsigned int entityId, void *data) {
	InputEvent *inputEvent = (InputEvent*)data;
	InputComponent *input = &getComponent(entityId, COMPONENT_INPUT)->input;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	if (!strcmp(inputEvent->keyName, "Left")) {
		/*MovementEvent moveInfo = {-5, 0};*/
		input->movingLeft = inputEvent->pressed;

		/*triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);*/
	} else if (!strcmp(inputEvent->keyName, "Right")) {
		input->movingRight = inputEvent->pressed;
		/*MovementEvent moveInfo = {5, 0};*/

		/*triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);*/
	}

	if (!strcmp(inputEvent->keyName, "Up")) {
		//triggerEvent(entityId, , &moveInfo);
	} else if (!strcmp(inputEvent->keyName, "Down")) {
	}
}

unsigned int createActor(int x, int y, int team) {
	unsigned int entityId = createEntity();

	registerHealth(entityId);
	registerWorldPosition(entityId, x, y);
	registerSprite(entityId, "src/tests/data/sprites/test.png");
	registerPhysics(entityId);
	registerEntityEvent(entityId, EVENT_TICK, &actorGridMoveHandler);
	addEntityToScene("action", entityId);

	SetPositionEvent pos = {x, y};

	triggerEvent(entityId, EVENT_SET_POSITION, &pos);

	return entityId;
}

unsigned int createPlayer(int x, int y, int team) {
	unsigned int entityId = createActor(x, y, team);

	registerKeyboardInput(entityId);
	registerEntityEvent(entityId, EVENT_KEY_INPUT, &actorKeyInputHandler);

	return entityId;
}
