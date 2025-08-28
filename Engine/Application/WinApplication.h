#pragma once

#include "stdafx.h"

struct GLFWwindow;

class WinApplication {

public:
    //WinApplication();
    static GLFWwindow* GetWindow() { return window; }
    void destroyWindow(GLFWwindow* window);
    ~WinApplication();
    static WinApplication* Get() { return m_windowApp; }

    static void Update();

    static void Init(void* appPointer);

    /*static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }*/

private:
    static GLFWwindow* window;
    static WinApplication* m_windowApp;

};