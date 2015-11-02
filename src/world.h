#include <stdlib.h>
#include "component.h"


typedef struct {
	unsigned int *entityMask, *deletedEntityIds;
	unsigned int *entityIdsToDelete;
	unsigned int **systemMask;
	unsigned int **entityEventCallbackCount;
	unsigned int entityCount, entityCountMax;
	unsigned int deletedEntityCount;
	unsigned int entityIdsToDeleteCount;
	unsigned int systemCountMax;
	unsigned int eventCount;
	unsigned int componentCount;
	unsigned int *systemIndex;
	void **systems;
	void (***systemCallback)(unsigned int, void*);
	void (****entityEventCallback)(unsigned int, void*);
	ComponentContainer **components;
	char *name;
} World;


void initWorlds(void);
void destroyWorlds(void);
void createWorld(char*);
void killWorld(void);
World* getWorld(void);
void setWorld(char*);
void cleanWorld(void);
