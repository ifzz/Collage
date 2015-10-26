#include "world.h"
#include "entity.h"

typedef struct {
	int ownerId; //For -1
	int targetId; //For -1
	int miscValue;
} TestComponent;

unsigned int COMPONENT_TEST_COMP = 0;

int main() {
	initWorlds();
	createWorld("World1");
	setWorld("World1");

	addComponentToWorld(&COMPONENT_TEST_COMP, sizeof(TestComponent));

	unsigned int entityId = createEntity();

	addComponentToEntity(entityId, COMPONENT_TEST_COMP);
	deleteEntity(entityId);
	cleanWorld();
	
	entityId = createEntity();
}
