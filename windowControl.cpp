#include "stdafx.h"
#include "WindowControl.h"

// Static member definitions
GLFWwindow* WindowControl::window = nullptr;
WindowControl* WindowControl::m_window = nullptr;

void WindowControl::destroyWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
}

WindowControl::~WindowControl() {
    if (window != nullptr) {
        destroyWindow(window);
    }
}

void WindowControl::Init(void* appPointer)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, appPointer);
}
