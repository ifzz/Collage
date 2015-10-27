#include "componentHealth.h"
#include "constants.h"
#include "../system.h"
#include "../component.h"

void testEvent1Callback(void*);
void testEvent2Callback(void*);


void initComponentHealth() {
	addComponentToWorld(&COMPONENT_STATS, sizeof(HealthComponent));
	createEvent(&EVENT_HIT);
	createEvent(&EVENT_DAMAGE);
	createSystem(EVENT_HIT, COMPONENT_STATS, testEvent1Callback);
	createSystem(EVENT_DAMAGE, COMPONENT_STATS, testEvent2Callback);
}

void registerHealth(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_STATS);
	HealthComponent *hC = getComponent(entityId, COMPONENT_STATS);

	hC->hp = 15;
}

void testEvent1Callback(void *data) {
	DamageEvent *tC = data;
	DamageEvent dE = {tC->targetId, tC->ownerId, tC->damage};

	printf("%u: Hitting %u for %i damage!\n", tC->ownerId, tC->targetId, tC->damage);

	triggerEvent(tC->targetId, EVENT_DAMAGE, &dE);
}   

void testEvent2Callback(void *data) {
	DamageEvent *dE = data;
	HealthComponent *hC = getComponent(dE->ownerId, COMPONENT_STATS);
	
	hC->hp -= dE->damage;

	printf("%u: Ouch! Hit for %i damage. Total HP: %i\n", dE->ownerId, dE->damage, hC->hp);
}

