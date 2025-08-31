#include "stdafx.h"
#include "WinApplication.h"
#include <GLFW/glfw3.h>

namespace StudyEngine {
    // Static member definitions
    GLFWwindow* WinApplication::window = nullptr;
    WinApplication* WinApplication::m_windowApp = nullptr;

    GLFWwindow* WinApplication::GetWindow()
    {
        return window;
    }

    void WinApplication::destroyWindow(GLFWwindow* window) {
        glfwTerminate();

        glfwDestroyWindow(window);
    }

    WinApplication::~WinApplication() {
        if (window != nullptr) {
            destroyWindow(window);
        }
    }

    WinApplication* WinApplication::Get()
    {
        return m_windowApp;
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
};


