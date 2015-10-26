#include <stdlib.h>


typedef struct {
	unsigned int *entityMask, *deletedEntityIds;
	unsigned int *entityIdsToDelete;
	unsigned int **systemMask;
	unsigned int entityCount, entityCountMax;
	unsigned int deletedEntityCount;
	unsigned int entityIdsToDeleteCount;
	unsigned int systemCountMax;
	unsigned int eventCount;
	unsigned int componentCount;
	unsigned int *systemIndex;
	void **systems;
	void (***systemCallback)(void*);
	void **components;
	char *name;
} World;


void initWorlds(void);
void createWorld(char*);
World* getWorld(void);
void setWorld(char*);
void cleanWorld(void);
