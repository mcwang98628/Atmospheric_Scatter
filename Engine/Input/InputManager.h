#pragma once
#include "stdafx.h"
#include "IInputEventHandler.h"
#include "InputEventTypes.h"

// Forward declaration instead of include
struct GLFWwindow;

namespace StudyEngine {
    class STUDY_ENGINE InputManager {
    public:
        static void Init();
        static void RegisterHandler(IInputEventHandler* handler);
        static void UnregisterHandler(IInputEventHandler* handler);
        static void ProcessEvents(const InputEvent& event);

        static bool IsKeyPressed(InputEvents key);
        //static InputManager* Instance()
        //{
        //    if (!m_inputManager)
        //    {
        //        m_inputManager = new InputManager();
        //        return m_inputManager;
        //    }
        //    return m_inputManager;
        //}

        static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void EnableMouseCallback(bool enable);

    private:
        //static InputManager* m_inputManager;
        static std::vector<IInputEventHandler*> m_handlers;
        static std::unordered_map<int, bool> m_keyStates;
        static InputEvents ConvertGLFWKeyToEngineKey(int glfwkey);
        static int ConvertEngineToGLFWKey(InputEvents key);
    };
};
