#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
	int targetId; //For -1
	int damage;
} DamageEvent;

typedef struct {
	short int team;
} SetTeamEvent;

typedef struct {
	int hp;
	short int team;
} HealthComponent;

#endif

void initComponentHealth(void);
void registerHealth(unsigned int);

