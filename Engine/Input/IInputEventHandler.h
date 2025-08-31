#pragma once

namespace StudyEngine {
    class IInputEventHandler {
    public:
        virtual ~IInputEventHandler() = default;
        virtual bool OnKeyPressed(int keyCode) = 0;
        virtual bool OnKeyReleased(int keyCode) = 0;
        virtual bool OnMousePressed(int button, float x, float y) = 0;
        virtual bool OnMouseReleased(int button, float x, float y) = 0;
        virtual bool OnMouseMoved(float x, float y) = 0;
        virtual bool OnMouseScrolled(float deltaX, float deltaY) = 0;
    };
}