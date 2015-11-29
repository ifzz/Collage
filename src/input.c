#include <stdio.h>
#include "input.h"
#include "system.h"
#include "component.h"

//void eventSetWorldPositionCallback(unsigned int, void*);


void initInput() {
	/*addComponentToWorld(&COMPONENT_INPUT, sizeof(InputComponent));*/

	createEvent(&EVENT_KEY_INPUT);
	createEvent(&EVENT_CONTROLLER_INPUT);

	//createSystem(EVENT_KEY_INPUT, COMPONENT_INPUT, eventSetWorldPositionCallback);
	//createSystem(EVENT_DAMAGE, COMPONENT_STATS, eventDamageCallback);
}

void registerKeyboardInput(unsigned int entityId) {
	addComponentToEntity(entityId, COMPONENT_INPUT);

	/*InputComponent *input = &getComponent(entityId, COMPONENT_INPUT)->input;*/

	/*input->isKeyboard = true;*/
}

