#include "stdafx.h"
#include "InputManager.h"
#include "../Application/WinApplication.h"
#include <GLFW/glfw3.h>


InputManager* InputManager::m_inputManager = nullptr;
std::vector<IInputEventHandler*> InputManager::m_handlers = std::vector<IInputEventHandler*>();


InputManager::InputManager()
{
	if (m_handlers.size())
	{
		m_handlers.clear();
	}
	m_handlers = std::vector<IInputEventHandler*>();
}

void InputManager::Init()
{
	glfwSetKeyCallback(WinApplication::GetWindow(), GLFWKeyCallback);
	glfwSetMouseButtonCallback(WinApplication::GetWindow(), GLFWMouseButtonCallback);
	glfwSetCursorPosCallback(WinApplication::GetWindow(), GLFWCursorPosCallback);
	glfwSetScrollCallback(WinApplication::GetWindow(), GLFWScrollCallback);
}
void InputManager::RegisterHandler(IInputEventHandler* handler)
{
	m_handlers.push_back(handler);
}

void InputManager::UnregisterHandler(IInputEventHandler* handler)
{
	m_handlers.erase(std::remove(m_handlers.begin(), m_handlers.end(), handler));
}

void InputManager::ProcessEvents(const InputEvent& event)
{
	for (auto handler : m_handlers)
	{
		bool comsumed = false;
		switch (event.type)
		{
		case InputEventType::KeyPressed:
			handler->OnKeyPressed(event.key.keyCode);
			break;
		case InputEventType::KeyReleased:
			handler->OnKeyReleased(event.key.keyCode);
			break;
		case InputEventType::MouseMoved:
			handler->OnMouseMoved(event.mouse.x, event.mouse.y);
			break;
		case InputEventType::MousePressed:
			handler->OnMousePressed(event.mouse.button, event.mouse.x, event.mouse.y);
			break;
		case InputEventType::MouseReleased:
			handler->OnMouseReleased(event.mouse.button, event.mouse.x, event.mouse.y);
			break;
		case InputEventType::MouseScrolled:
			handler->OnMouseScrolled(event.scroll.deltaX, event.scroll.deltaY);
		default:
			break;
		}
	}
}

void InputManager::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputEvent evt;
	evt.type = (action == GLFW_PRESS) ? InputEventType::KeyPressed : InputEventType::KeyReleased;
	evt.key.keyCode = static_cast<int>(ConvertGLFWKeyToEngineKey(key));
	Instance()->ProcessEvents(evt);
}

void InputManager::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputEvent evt;
	evt.type = (action == GLFW_PRESS) ? InputEventType::MousePressed : InputEventType::MouseReleased;
	evt.mouse.button = button;
	double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    evt.mouse.x = static_cast<float>(xpos);
    evt.mouse.y = static_cast<float>(ypos);
	Instance()->ProcessEvents(evt);
}

void InputManager::GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputEvent evt;

	evt.type = InputEventType::MouseMoved;
	//glfwGetCursorPos(window, &xpos, &ypos);
	evt.mouseMove.x = static_cast<float>(xpos);
	evt.mouseMove.y = static_cast<float>(ypos);

	Instance()->ProcessEvents(evt);
}

void InputManager::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputEvent evt;

	evt.type = InputEventType::MouseScrolled;
	evt.scroll.deltaX = static_cast<float>(xoffset);
	evt.scroll.deltaY = static_cast<float>(yoffset);

	Instance()->ProcessEvents(evt);
}

InputEvents InputManager::ConvertGLFWKeyToEngineKey(int glfwkey)
{
	switch (glfwkey)
	{
	case GLFW_KEY_W:
		return InputEvents::Key_W;
	break;
	case GLFW_KEY_S:
		return InputEvents::Key_S;
	break;
	case GLFW_KEY_A:
		return InputEvents::Key_A;
	break;
	case GLFW_KEY_D:
		return InputEvents::Key_D;
	break;
	case GLFW_KEY_ESCAPE:
		return InputEvents::Key_ESCAPE;
	break;
	case GLFW_KEY_UP:
		return InputEvents::Key_UP;
	break;
	case GLFW_KEY_DOWN:
		return InputEvents::Key_DOWN;
	break;
	case GLFW_KEY_LEFT:
		return InputEvents::Key_LEFT;
	break;
	case GLFW_KEY_RIGHT:
		return InputEvents::Key_RIGHT;
	break;
	case GLFW_KEY_Q:
		return InputEvents::Key_Q;
	break;
	case GLFW_KEY_E:
		return InputEvents::Key_E;
	break;
	default:
		return InputEvents::UNKNOWN;
	}
	return InputEvents::UNKNOWN;
}
