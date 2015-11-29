#include <stdio.h>
#include "../component.h"
#include "../entity.h"
#include "../sprite.h"
#include "../system.h"
#include "../input.h"
#include "../scene.h"
#include "../worldPosition.h"
#include "components/health.h"
#include "components/physics.h"
#include "constants.h"
#include "abilities.h"
#include "actors.h"


void actorKeyInputHandler(unsigned int entityId, void *data) {
	InputEvent *input = (InputEvent*)data;
	PhysicsComponent *physics = &getComponent(entityId, COMPONENT_PHYSICS)->physics;

	if (!strcmp(input->keyName, "Left")) {
		//triggerEvent(entityId, , &moveInfo);
	} else if (!strcmp(input->keyName, "Right")) {
	}

	if (!strcmp(input->keyName, "Up")) {
		//triggerEvent(entityId, , &moveInfo);
	} else if (!strcmp(input->keyName, "Down")) {
	}
}

unsigned int createActor(int x, int y, int team) {
	unsigned int entityId = createEntity();

	registerHealth(entityId);
	registerWorldPosition(entityId, x, y);
	registerSprite(entityId, "src/tests/data/sprites/test.png");
	registerPhysics(entityId);
	addEntityToScene(entityId);

	SetPositionEvent pos = {x, y};

	triggerEvent(entityId, EVENT_SET_POSITION, &pos);

	return entityId;
}

unsigned int createPlayer(int x, int y, int team) {
	unsigned int entityId = createActor(x, y, team);

	//registerKeyboardInput(entityId);
	//registerEntityEvent(entityId, EVENT_KEY_INPUT, &actorKeyInputHandler);

	return entityId;
}
