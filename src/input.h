#include <stdbool.h>


unsigned int COMPONENT_INPUT;
unsigned int EVENT_KEY_INPUT;
unsigned int EVENT_CONTROLLER_INPUT;

#ifndef INPUT_H
#define INPUT_H

typedef struct {
	const char *keyName;
} InputEvent;

typedef struct {
	bool isKeyboard;
	int controllerIndex;
} InputComponent;

#endif

void initInput(void);
void registerKeyboardInput(unsigned int);

