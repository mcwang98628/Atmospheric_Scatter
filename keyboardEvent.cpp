#include "keyboardEvent.h"

int KeyboardEvent::horizontalVelocity = 0;
int KeyboardEvent::verticalVelocity = 0;

void KeyboardEvent::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		verticalVelocity = 10;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		verticalVelocity = -10;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		horizontalVelocity = 10;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		horizontalVelocity = -10;
	}
	if (action == GLFW_RELEASE) {
		horizontalVelocity = 0;
		verticalVelocity = 0;
	}
}