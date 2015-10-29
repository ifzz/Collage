#include <stdio.h>
#include "worldPosition.h"
#include "../constants.h"
#include "../../system.h"
#include "../../component.h"

//void eventHitCallback(void*);
//void eventDamageCallback(void*);


void initComponentWorldPosition() {
	addComponentToWorld(&COMPONENT_WORLD_POSITION, sizeof(WorldPositionComponent));
	//createEvent(&EVENT_HIT);
	//createEvent(&EVENT_DAMAGE);
	//createSystem(EVENT_HIT, COMPONENT_STATS, eventHitCallback);
	//createSystem(EVENT_DAMAGE, COMPONENT_STATS, eventDamageCallback);
}

void registerWorldPosition(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_WORLD_POSITION);
	WorldPositionComponent *hC = getComponent(entityId, COMPONENT_WORLD_POSITION);

	//hC->hp = 15;
}

//void eventHitCallback(void *data) {
	//DamageEvent *tC = data;
	//DamageEvent dE = {tC->targetId, tC->ownerId, tC->damage};

	//printf("%u: Hitting %u for %i damage!\n", tC->ownerId, tC->targetId, tC->damage);

	//triggerEvent(tC->targetId, EVENT_DAMAGE, &dE);
//}   

//void eventDamageCallback(void *data) {
	//DamageEvent *dE = data;
	//HealthComponent *hC = getComponent(dE->ownerId, COMPONENT_STATS);
	
	//hC->hp -= dE->damage;

	//printf("%u: Ouch! Hit for %i damage. Total HP: %i\n", dE->ownerId, dE->damage, hC->hp);
//}

