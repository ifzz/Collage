#ifndef WORLD_POSITION_H
#define WORLD_POSITION_H

typedef struct {
	int vX;
	int vY;
} MoveEvent;

typedef struct {
	int x;
	int y;
} WorldPositionComponent;

#endif

void initComponentWorldPosition(void);
void registerWorldPosition(unsigned int);

