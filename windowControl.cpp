#include "WindowControl.h"

WindowControl::WindowControl(void* appPointer) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, appPointer);
}

void WindowControl::destroyWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
}

WindowControl::~WindowControl() {
    if (window != NULL) {
        destroyWindow(window);
    }
}
