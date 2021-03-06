#include <stdlib.h>
#include "component.h"


typedef struct {
	unsigned int *entityMask, *deletedEntityIds;
	unsigned int *entityIdsToDelete;
	unsigned int **systemMask;
	int **entityEventCallbackCount;
	int entityCount, entityCountMax;
	int highestEntityId;
	int deletedEntityCount;
	int entityIdsToDeleteCount;
	int systemCountMax;
	int eventCount;
	int componentCount;
	unsigned int *systemIndex;
	void **systems;
	void (***systemCallback)(unsigned int, void*);
	void (****entityEventCallback)(unsigned int, void*);
	void **components;
	char componentNames[32][MAX_COMPONENT_NAME_LEN];
	char name[25];
} World;


void initWorlds(void);
void destroyWorlds(void);
void createWorld(char*);
void killWorld(void);
World* getWorld(void);
void setWorld(char*);
void cleanWorld(void);
