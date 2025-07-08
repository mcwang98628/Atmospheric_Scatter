#pragma once

#include "stdafx.h"


class WindowControl {

public:
    //WindowControl();
    static GLFWwindow* GetWindow() { return window; }
    void destroyWindow(GLFWwindow* window);
    ~WindowControl();
    static WindowControl* Get() { return m_window; }

    static void Init(void* appPointer);

private:
    static GLFWwindow* window;
    static WindowControl* m_window;

};