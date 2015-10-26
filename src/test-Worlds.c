#include <stdio.h>
#include "world.h"
#include "system.h"
#include "entity.h"

typedef struct {
	int ownerId; //For -1
	int targetId; //For -1
	int miscValue;
} TestComponent;

unsigned int COMPONENT_TEST_COMP = 0;
unsigned int TEST_EVENT_1 = 0;

void testEvent1Callback(void *data) {
	TestComponent *tC = data;

	printf("%i, %i, %i\n", tC->ownerId, tC->targetId, tC->miscValue);
}   

int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");

	addComponentToWorld(&COMPONENT_TEST_COMP, sizeof(TestComponent));
	createEvent(&TEST_EVENT_1);
	createSystem(TEST_EVENT_1, COMPONENT_TEST_COMP, testEvent1Callback);

	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_TEST_COMP);

	printf("Trigger\n");
	TestComponent tC = {entityId, -1, 10};
	triggerEvent(entityId, TEST_EVENT_1, &tC);

}
