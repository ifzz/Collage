#include <stdio.h>
#include "../entity.h"
#include "components/health.h"
#include "constants.h"
#include "actors.h"


//#TODO: Move to abilities.c
void activateHunterAbility(void *data) {
	printf("Activated ability!\n");
}

unsigned int createHunter(int x, int y, int team) {
	unsigned int entityId = createEntity();

	registerHealth(entityId);
	registerEntityEvent(entityId, EVENT_DAMAGE, &activateHunterAbility);

	return entityId;
}
