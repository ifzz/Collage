#include <stdio.h>


typedef struct {
	unsigned int ownerId;
	unsigned int targetId;
} ActorInteraction1;

typedef struct {
	unsigned int ownerId;
	unsigned int targetId;
	int value1;
} ActorInteraction2;


// What can we do with return values?
int systemInteraction1(void *data) {
	ActorInteraction1 *aI = data;

	printf("Sys1: \toId=%i, tId=%i\n", aI->ownerId, aI->targetId);

	return 0;
}

int systemInteraction2(void *data) {
	ActorInteraction2 *aI = data;

	printf("Sys2: \toId=%i, tId=%i v1=%i\n", aI->ownerId, aI->targetId, aI->value1);

	return 0;
}

void triggerEvent(int world, unsigned int entityId, char *event, void *data) {
	systemInteraction1(data);
	systemInteraction2(data);
}


int main() {
	ActorInteraction1 aI1 = {1001, 1002};

	triggerEvent(0, 0, "test", &aI1);
	
	return 0;
}
