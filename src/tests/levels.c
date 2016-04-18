#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "levels.h"
#include "actors.h"
#include "items.h"


short int *LEVEL = NULL;
short int CLEAR = 0, SOLID = 1, LADDER = 2;
int LEVEL_WIDTH, LEVEL_HEIGHT;


void createLevel(int width, int height) {
	LEVEL = (short int*)calloc((width + 1) * (height + 1), sizeof(short int));
	LEVEL_WIDTH = width;
	LEVEL_HEIGHT = height;

	int playerX = -1, playerY = -1;

	printf("Level size: %ix%i\n", LEVEL_WIDTH, LEVEL_HEIGHT);
	int x = 0, y = 0, c;

	FILE *levelFile = fopen ("src/tests/data/levels/001.txt", "r");

	while((c = fgetc(levelFile)) != EOF) {
		if (c == '@') {
			playerX = x;
			playerY = y;
		} else if (c == '#') {
			setPositionSolid(x, y);
		} else if (c == 'H') {
			setPositionLadder(x, y);
		} else if (c == '*') {
			createCollectible(x, y, "src/tests/data/sprites/gold.png");
		} else if (c == '\n') {
			x = 0;
			++ y;
		}

		++ x;
	}

	assert(playerX > -1 && playerY > -1);
	createPlayer(playerX, playerY, 1);
	fclose(levelFile);
}

void destroyLevel() {
	if (LEVEL != NULL)
		free(LEVEL);
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
