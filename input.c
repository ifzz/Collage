#include <stdio.h>
#include <SDL2/SDL.h>
#include "input.h"
#include "system.h"
#include "component.h"


SDL_Event EVENT;
//void eventSetWorldPositionCallback(unsigned int, void*);


void initInput() {
	addComponentToWorld("input", &COMPONENT_INPUT, sizeof(InputComponent));

	createEvent(&EVENT_KEY_INPUT);
	createEvent(&EVENT_MOUSEWHEEL_INPUT);
	createEvent(&EVENT_MOUSEBUTTON_INPUT);
	createEvent(&EVENT_CONTROLLER_INPUT);

	HARD_EXIT = 0;

	//createSystem(EVENT_KEY_INPUT, COMPONENT_INPUT, eventSetWorldPositionCallback);
}

void registerKeyboardInput(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_INPUT);

	InputComponent *input = getComponent(entityId, COMPONENT_INPUT);

	input->isKeyboard = true;
}

void handleKeyInput(SDL_KeyboardEvent *key, bool pressed) {
	//TODO: More key info here.
	InputEvent keyEvent = {SDL_GetKeyName(key->keysym.sym), pressed};

	triggerEventForAll(EVENT_KEY_INPUT, COMPONENT_INPUT, &keyEvent);

	if (key->keysym.sym == SDLK_ESCAPE)
		HARD_EXIT = 1;
}

void handleMouseButton(SDL_MouseButtonEvent *mouseButton, bool pressed) {
	InputEvent keyEvent = {"", pressed};

	if (mouseButton->button == SDL_BUTTON_LEFT)
		keyEvent.keyName = "Mouse1";
	else if (mouseButton->button == SDL_BUTTON_RIGHT)
		keyEvent.keyName = "Mouse2";

	triggerEventForAll(EVENT_MOUSEBUTTON_INPUT, COMPONENT_INPUT, &keyEvent);
}

void handleMouseWheel(SDL_MouseWheelEvent *mouseWheel) {
	triggerEventForAll(EVENT_MOUSEWHEEL_INPUT, COMPONENT_INPUT, mouseWheel);
}

void inputLoop() {
	while (SDL_PollEvent(&EVENT)) {
		switch (EVENT.type) {
			case SDL_QUIT:
				HARD_EXIT = 1;

				return;
			case SDL_KEYDOWN:
				handleKeyInput(&EVENT.key, true);

				break;
			case SDL_KEYUP:
				handleKeyInput(&EVENT.key, false);
				/*handleKeyRelease(&EVENT.key);*/

				break;
			case SDL_MOUSEBUTTONDOWN:
				handleMouseButton(&EVENT.button, true);

				break;
			case SDL_MOUSEBUTTONUP:
				handleMouseButton(&EVENT.button, false);

				break;
			case SDL_MOUSEWHEEL:
				handleMouseWheel(&EVENT.wheel);

				break;
			case SDL_JOYAXISMOTION:
				/*handleControllerAxisTilt(&EVENT.caxis);*/

				break;
			case SDL_JOYBUTTONDOWN:
				/*handleControllerButtonPress(&EVENT.cbutton);*/

				break;
			case SDL_JOYBUTTONUP:
				/*handleControllerButtonRelease(&EVENT.cbutton);*/

				break;
			case SDL_JOYHATMOTION:
				/*handleControllerHatMotion(&EVENT.jhat);*/

				break;
			case SDL_JOYDEVICEADDED:
				/*handleConnectJoystick(&EVENT.cdevice);*/

				break;
			case SDL_JOYDEVICEREMOVED:
				/*handleDisconnectJoystick(&EVENT.cdevice);*/

				break;
			default:
				break;
		}
	}
}
