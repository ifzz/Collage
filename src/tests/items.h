#ifndef ITEMS_H
#define ITEMS_H

typedef struct {
	unsigned int targetId;
} CollisionEvent;

#endif


void initItems(void);
unsigned int createItem(int, int, char*);
unsigned int createCollectible(int, int, char*);
