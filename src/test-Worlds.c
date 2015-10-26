#include <stdio.h>
#include "world.h"
#include "system.h"
#include "entity.h"
#include "component.h"

typedef struct {
	int ownerId; //For -1
	int targetId; //For -1
	int miscValue;
} TestComponent;

typedef struct {
	int hp;
} HealthComponent;

unsigned int COMPONENT_TEST_COMP = 0;
unsigned int TEST_EVENT_1 = 0;


void testEvent1Callback(void *data) {
	TestComponent *tC = data;

	HealthComponent *hC = getComponent(tC->ownerId, COMPONENT_TEST_COMP);
	
	printf("%i\n", hC->hp);
}   

int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");

	addComponentToWorld(&COMPONENT_TEST_COMP, sizeof(HealthComponent));
	createEvent(&TEST_EVENT_1);
	createSystem(TEST_EVENT_1, COMPONENT_TEST_COMP, testEvent1Callback);

	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_TEST_COMP);

	TestComponent tC = {entityId, -1, 10};
	HealthComponent *hC = getComponent(entityId, COMPONENT_TEST_COMP);

	hC->hp = 10;
	triggerEvent(entityId, TEST_EVENT_1, &tC);

}
