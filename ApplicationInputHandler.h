#pragma once
#include "Engine/Input/IInputEventHandler.h"
#include "Engine/Input/InputEventTypes.h"
#include <functional>
#include <unordered_map>
#include <vector>


class ApplicationInputHandler : public IInputEventHandler
{
private:
    std::unordered_map<int, std::function<void()>> m_keyBindings;
    std::vector<std::function<void(float, float)>> m_mouseMoveBindings;
public:
    void BindKey(InputEvents keyCode, std::function<void()> callback);
    void BindMouseMove(std::function<void(float, float)> callback);
    bool OnKeyPressed(int keyCode) override;
    bool OnKeyReleased(int keyCode) override;
    bool OnMousePressed(int button, float x, float y) override;
    bool OnMouseReleased(int button, float x, float y) override;
    bool OnMouseMoved(float x, float y) override;
    bool OnMouseScrolled(float deltaX, float deltaY) override;

};