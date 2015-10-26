#include <stdio.h>
#include "framework/display.h"
#include "world.h"
#include "system.h"
#include "entity.h"
#include "component.h"
#include "timestep.h"

typedef struct {
	int ownerId; //For -1
	int targetId; //For -1
	int damage;
} DamageEvent;

typedef struct {
	int hp;
} HealthComponent;

unsigned int COMPONENT_STATS = 0;
unsigned int EVENT_HIT = 0;
unsigned int EVENT_DAMAGE = 0;


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

void registerObject(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_STATS);
	HealthComponent *hC = getComponent(entityId, COMPONENT_STATS);

	hC->hp = 15;
}

int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");
	
	initTimestep();
	//displayInit();

	addComponentToWorld(&COMPONENT_STATS, sizeof(HealthComponent));
	createEvent(&EVENT_HIT);
	createEvent(&EVENT_DAMAGE);
	createSystem(EVENT_HIT, COMPONENT_STATS, testEvent1Callback);
	createSystem(EVENT_DAMAGE, COMPONENT_STATS, testEvent2Callback);

	unsigned int entityId = createEntity();
	unsigned int targetId = createEntity();

	registerObject(entityId);
	registerObject(targetId);

	DamageEvent tC = {entityId, targetId, 10};

	triggerEvent(entityId, EVENT_HIT, &tC);
	stepTime();
	//triggerEvents(COMPONENT_TEST_COMP, EVENT_TICK, NULL);
}
