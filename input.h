#include <stdbool.h>


unsigned int COMPONENT_INPUT;
unsigned int EVENT_KEY_INPUT;
unsigned int EVENT_CONTROLLER_INPUT;
bool HARD_EXIT;

#ifndef INPUT_H
#define INPUT_H

typedef struct {
	const char *keyName;
	bool pressed;
} InputEvent;

typedef struct {
	bool isKeyboard;
	bool movingLeft, movingRight, movingUp, movingDown;
	int controllerIndex;
} InputComponent;

#endif

void initInput(void);
void inputLoop(void);
void registerKeyboardInput(unsigned int);

