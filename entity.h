unsigned int EVENT_DELETE;

void initEntities(void);
unsigned int createEntity(void);
void deleteEntity(unsigned int);
void registerEntityEvent(unsigned int, unsigned int,
		void (*)(unsigned int, void*));
void unregisterEntityEvent(unsigned int, unsigned int,
		void (*)(unsigned int, void*));
void unregisterAllEntityEvents(unsigned int, unsigned int);
void triggerEvent(unsigned int, unsigned int, void*);
