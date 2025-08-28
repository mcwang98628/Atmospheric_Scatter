#include "stdafx.h"
#include "WinApplication.h"
#include <GLFW/glfw3.h>


// Static member definitions
GLFWwindow* WinApplication::window = nullptr;
WinApplication* WinApplication::m_windowApp = nullptr;

void WinApplication::destroyWindow(GLFWwindow* window) {
    glfwTerminate();

    glfwDestroyWindow(window);
}

WinApplication::~WinApplication() {
    if (window != nullptr) {
        destroyWindow(window);
    }
}

void WinApplication::Update()
{
    glfwPollEvents();
}

void WinApplication::Init(void* appPointer)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, appPointer);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

}
