typedef struct {
	int ownerId; //For -1
	int targetId; //For -1
	int damage;
} DamageEvent;

typedef struct {
	int hp;
} HealthComponent;

void initComponentHealth(void);
void registerHealth(unsigned int);

