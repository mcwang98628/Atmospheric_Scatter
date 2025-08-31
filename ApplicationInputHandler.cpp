#include "ApplicationInputHandler.h"
#include <iostream>

void ApplicationInputHandler::BindKey(InputEvents keyCode, std::function<void()> callback)
{
    m_keyBindings[static_cast<int>(keyCode)] = callback;
}

void ApplicationInputHandler::BindMouseMove(std::function<void(float, float)> callback)
{
    m_mouseMoveBindings.push_back(callback);
}

bool ApplicationInputHandler::OnKeyPressed(int keyCode)
{
    if(auto it = m_keyBindings.find(keyCode); it != m_keyBindings.end())
    {
        it->second();
        return true;
    }
    // } else {
    //     std::cout << "Not some key pressed"<<  keyCode << std::endl;
    // }
    return false;
}

bool ApplicationInputHandler::OnKeyReleased(int keyCode)
{
    return false;
}

bool ApplicationInputHandler::OnMousePressed(int button, float x, float y)
{
    return false;
}

bool ApplicationInputHandler::OnMouseReleased(int button, float x, float y)
{
    return false;
}

bool ApplicationInputHandler::OnMouseMoved(float x, float y)
{
    if(m_mouseMoveBindings.size())
    {
        for(auto& callback : m_mouseMoveBindings)
        {
            callback(x, y);
        }
        return true;
    }
    return false;
}

bool ApplicationInputHandler::OnMouseScrolled(float deltaX, float deltaY)
{
    return false;
}
