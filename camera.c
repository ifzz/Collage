#include "timestep.h"
#include "system.h"


void renderCamera(unsigned int, void *data);


void initCameras() {
	addComponentToWorld(&COMPONENT_CAMERA, sizeof(CameraComponent));

	createSystem(EVENT_TIMESTEP_RENDER, COMPONENT_CAMERA, renderCamera);
}

void renderCamera() {
}
