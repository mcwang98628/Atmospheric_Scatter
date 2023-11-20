#include "WindowInit.h"

WindowInit::WindowInit(void* appPointer) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, appPointer);
}

GLFWwindow* WindowInit::initWindow(void* appPointer) {
    GLFWwindow* window;
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, appPointer);
    // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    return window;
}


void WindowInit::destroyWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
}

WindowInit::~WindowInit() {
    destroyWindow(window);
}
