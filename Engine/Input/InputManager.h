#pragma once
#include "IInputEventHandler.h"
#include "InputEventTypes.h"

// Forward declaration instead of include
struct GLFWwindow;


class InputManager {
public:
    InputManager();
    void Init();
    void RegisterHandler(IInputEventHandler* handler);
    void UnregisterHandler(IInputEventHandler* handler);
    void ProcessEvents(const InputEvent& event);

    static InputManager* Instance()
    {
        if (!m_inputManager)
        {
            m_inputManager = new InputManager();
            return m_inputManager;
        }
        return m_inputManager;
    }

    static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    static InputManager* m_inputManager;
    static std::vector<IInputEventHandler*> m_handlers;
    static InputEvents ConvertGLFWKeyToEngineKey(int glfwkey);
};