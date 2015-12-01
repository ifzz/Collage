#include <stdio.h>
#include "../framework/display.h"
#include "../framework/numbers.h"
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
#include "levels.h"


void actorGridMoveHandler(unsigned int entityId, void *data) {
	InputComponent *input = &getComponent(entityId, COMPONENT_INPUT)->input;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;
	WorldPositionComponent *position = &getComponent(entityId, COMPONENT_WORLD_POSITION)->worldPosition;

	bool ladderBelow = isPositionLadder((position->x / 16), (position->y / 16) + 1);
	bool onGround = (position->x % 16) || isPositionSolid((position->x / 16), (position->y / 16) + 1) || ladderBelow;
	bool onLadder = !(position->x % 16) && (isPositionLadder((position->x / 16), (position->y / 16)));

	/*printf("%i, %i, %i\n", ladderBelow, onGround, onLadder);*/

	if (onGround) {
		if (!(position->y % 16)) {
			if (input->movingLeft && position->x > 16 * 1) {
				MovementEvent moveInfo = {-5, 0};

				triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
			} else if (input->movingRight && position->x < 16 * ((displayGetRenderWidth() / 16) - 1)) {
				MovementEvent moveInfo = {5, 0};

				triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
			} else if (!(position->x % 16)) {
				MovementEvent moveInfo = {0, physics->velocityY};

				triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
			}
		}
	}
	
	if (onLadder || ladderBelow) {
		if (input->movingUp && position->y > 16 * 1) {
			/*if (isPositionLadder((position->x / 16), (position->y / 16))) {*/
			if (onLadder) {
				MovementEvent moveInfo = {0, -5};

				triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
			}
		} else if (input->movingDown && position->y < 16 * ((displayGetRenderHeight() / 16) - 1)) {
			if (ladderBelow) {
				MovementEvent moveInfo = {0, 5};

				triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
			}
		} else if (!(position->y % 16)) {
			MovementEvent moveInfo = {physics->velocityX, 0};

			triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
		}
	}

	if (!onGround && !onLadder && !ladderBelow) {
		MovementEvent moveInfo = {0, 10};

		triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
	} else if (onGround && !ladderBelow) {
		MovementEvent moveInfo = {physics->velocityX, clipFloat(physics->velocityY, -5, 0)};

		SetPositionEvent positionInfo = {position->x, position->y - (position->y % 16)};
		physics->exactY = position->y - (position->y % 16);

		triggerEvent(entityId, EVENT_SET_POSITION, &positionInfo);
		/*printf("\tAfter: %i, %i %f\n", position->y, (position->y % 16), physics->velocityY);*/

		triggerEvent(entityId, EVENT_SET_VELOCITY, &moveInfo);
	}
}

void actorKeyInputHandler(unsigned int entityId, void *data) {
	InputEvent *inputEvent = (InputEvent*)data;
	InputComponent *input = &getComponent(entityId, COMPONENT_INPUT)->input;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	if (!strcmp(inputEvent->keyName, "Left")) {
		input->movingLeft = inputEvent->pressed;
	} else if (!strcmp(inputEvent->keyName, "Right")) {
		input->movingRight = inputEvent->pressed;
	}

	if (!strcmp(inputEvent->keyName, "Up")) {
		input->movingUp = inputEvent->pressed;
	} else if (!strcmp(inputEvent->keyName, "Down")) {
		input->movingDown = inputEvent->pressed;
	}
}

void actorAiInputHandler(unsigned int entityId, void *data) {
}

unsigned int createActor(int x, int y, int team) {
	unsigned int entityId = createEntity();

	x *= 16;
	y *= 16;

	registerHealth(entityId);
	registerWorldPosition(entityId, x, y);
	registerSprite(entityId, "src/tests/data/sprites/test.png");
	registerPhysics(entityId);
	registerEntityEvent(entityId, EVENT_TICK, &actorGridMoveHandler);
	addEntityToScene("action", entityId);

	SetPositionEvent pos = {x, y};

	triggerEvent(entityId, EVENT_SET_POSITION, &pos);
	triggerEvent(entityId, EVENT_SET_POSITION, &pos);

	return entityId;
}

unsigned int createPlayer(int x, int y, int team) {
	unsigned int entityId = createActor(x, y, team);

	registerKeyboardInput(entityId);
	registerEntityEvent(entityId, EVENT_KEY_INPUT, &actorKeyInputHandler);

	return entityId;
}
