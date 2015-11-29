#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "levels.h"
#include "items.h"


short int *LEVEL = NULL;
short int CLEAR = 0, SOLID = 1, LADDER = 2;
int LEVEL_WIDTH, LEVEL_HEIGHT;


void createLevel(int width, int height) {
	LEVEL = (short int*)calloc(width * height, sizeof(short int));
	LEVEL_WIDTH = width;
	LEVEL_HEIGHT = height;

	for (int x = 1; x < 5; ++ x) {
		setPositionSolid(x, 3);
		setPositionSolid(x + 3, 6);
	}

	for (int y = 3; y < 6; ++ y) {
		setPositionLadder(5, y);
	}
}

void setPositionSolid(int x, int y) {
	assert(LEVEL);

	LEVEL[x + (y * LEVEL_WIDTH)] = SOLID;

	createItem(x, y, "src/tests/data/sprites/ground.png");
}

void setPositionClear(int x, int y) {
	assert(LEVEL);

	LEVEL[x + (y * LEVEL_WIDTH)] = CLEAR;
}

void setPositionLadder(int x, int y) {
	assert(LEVEL);

	LEVEL[x + (y * LEVEL_WIDTH)] = LADDER;

	createItem(x, y, "src/tests/data/sprites/ladder.png");
}

bool isPositionSolid(int x, int y) {
	return LEVEL[x + (y * LEVEL_WIDTH)] == SOLID;
}

bool isPositionLadder(int x, int y) {
	return LEVEL[x + (y * LEVEL_WIDTH)] == LADDER;
}
