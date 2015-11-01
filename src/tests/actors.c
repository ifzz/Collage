#include <stdio.h>
#include "../entity.h"
#include "../sprite.h"
#include "../system.h"
#include "../scene.h"
#include "../worldPosition.h"
#include "components/health.h"
#include "constants.h"
#include "abilities.h"
#include "actors.h"


unsigned int createHunter(int x, int y, int team) {
	unsigned int entityId = createEntity();

	registerHealth(entityId);
	registerEntityEvent(entityId, EVENT_DAMAGE, &activateHunterAbility);
	registerWorldPosition(entityId, x, y);
	registerSprite(entityId, "src/tests/data/sprites/test.png");
	addEntityToScene(entityId);

	SetPositionEvent pos = {x, y};

	triggerEvent(entityId, EVENT_SET_POSITION, &pos);

	return entityId;
}
