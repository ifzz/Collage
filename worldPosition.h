unsigned int COMPONENT_WORLD_POSITION;
unsigned int EVENT_SET_POSITION;

#ifndef WORLD_POSITION_H
#define WORLD_POSITION_H

typedef struct {
	int x;
	int y;
} SetPositionEvent;

typedef struct {
	int lastX;
	int lastY;
	int x;
	int y;
} WorldPositionComponent;

#endif

void initComponentWorldPosition(void);
void registerWorldPosition(unsigned int, int, int);

