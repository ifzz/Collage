#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
	int ownerId; //For -1
	int targetId; //For -1
	int damage;
} DamageEvent;

typedef struct {
	int hp;
} HealthComponent;

#endif

void initComponentHealth(void);
void registerHealth(unsigned int);

