#pragma once

#include "stdafx.h"

struct GLFWwindow;
namespace StudyEngine {
    class STUDY_ENGINE WinApplication {

    public:
        //WinApplication();
        static GLFWwindow* GetWindow();
        static void destroyWindow(GLFWwindow* window);
        ~WinApplication();
        static WinApplication* Get();

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
};
