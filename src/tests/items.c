#include "components/health.h"
#include "../worldPosition.h"
#include "../timestep.h"
#include "../sprite.h"
#include "../system.h"
#include "../entity.h"
#include "../scene.h"
#include "../world.h"
#include "../input.h"
#include "constants.h"
#include "items.h"


void collisionHandler(unsigned int, void*);


void initItems() {
	createEvent(&EVENT_COLLISION);
}

void collisionHandler(unsigned int entityId, void *data) {
	World *world = getWorld();
	unsigned int actorMask = COMPONENT_STATS | COMPONENT_INPUT;
	WorldPositionComponent *position = &getComponent(entityId,
			COMPONENT_WORLD_POSITION)->worldPosition;

	for (unsigned int childEntityId = 0; childEntityId < world->entityCountMax;
			++ childEntityId) {
		if (entityId == childEntityId)
			continue;

		if ((world->entityMask[childEntityId] & actorMask) != actorMask)
			continue;

		WorldPositionComponent *childPosition = &getComponent(childEntityId,
				COMPONENT_WORLD_POSITION)->worldPosition;

		if (childPosition->x == position->x &&
				childPosition->y == position->y) {
			CollisionEvent collisionInfo = {childEntityId};

			triggerEvent(entityId, EVENT_COLLISION, &collisionInfo);
		}
	}
}

void itemLightingHandler(unsigned int entityId, void *data) {
	World *world = getWorld();
	unsigned int actorMask = COMPONENT_STATS | COMPONENT_INPUT;
	WorldPositionComponent *position = &getComponent(entityId,
			COMPONENT_WORLD_POSITION)->worldPosition;
	int closestDist = 10000;

	for (unsigned int childEntityId = 0; childEntityId < world->entityCountMax;
			++ childEntityId) {
		if (entityId == childEntityId)
			continue;

		if ((world->entityMask[childEntityId] & actorMask) != actorMask)
			continue;

		WorldPositionComponent *childPosition = &getComponent(childEntityId,
				COMPONENT_WORLD_POSITION)->worldPosition;

		int dist = abs(childPosition->x - position->x) +
			abs(childPosition->y - position->y);

		if (dist < closestDist)
			closestDist = dist;
	}

	SetAlphaEvent alphaEvent = {255};

	if (closestDist > 128) 
		alphaEvent.alpha = 0;

	triggerEvent(entityId, EVENT_SET_ALPHA, &alphaEvent);
}

unsigned int createItem(int x, int y, char *sprite) {
	unsigned int entityId = createEntity();

	registerWorldPosition(entityId, x, y);
	registerSprite(entityId, sprite);
	addEntityToScene("action", entityId);

	SetPositionEvent pos = {x * 16, y * 16};

	//Prevent flicker- do twice
	triggerEvent(entityId, EVENT_SET_POSITION, &pos);
	/*triggerEvent(entityId, EVENT_SET_POSITION, &pos);*/

	//Lighting
	/*registerEntityEvent(entityId, EVENT_TICK, &itemLightingHandler);*/

	return entityId;
}

void collectibleCollisionHandler(unsigned int entityId, void *data) {
	CollisionEvent *collisionInfo = (CollisionEvent*)data;

	deleteEntity(entityId);
}

unsigned int createCollectible(int x, int y, char *sprite) {
	unsigned int entityId = createItem(x, y, sprite);

	registerEntityEvent(entityId, EVENT_TICK, &collisionHandler);
	registerEntityEvent(entityId, EVENT_COLLISION,
			&collectibleCollisionHandler);

	return entityId;
}
