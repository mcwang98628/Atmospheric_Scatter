#include <GLFW/glfw3.h>

static class KeyboardEvent {
public:
	static int verticalVelocity;
	static int horizontalVelocity;
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};
