#include <stdlib.h>


typedef struct {
	unsigned int *entityMask, *deletedEntityIds;
	unsigned int *entityIdsToDelete;
	unsigned int entityCount, entityCountMax;
	unsigned int deletedEntityCount;
	unsigned int entityIdsToDeleteCount;
	unsigned int systemCount, systemCountMax;
	void **systems;
	char **systemNames;
	char *name;
} World;


void initWorlds(void);
void createWorld(char*);
void addComponentToWorld(unsigned int*, size_t);
World* getWorld(void);
void setWorld(char*);
void cleanWorld(void);
