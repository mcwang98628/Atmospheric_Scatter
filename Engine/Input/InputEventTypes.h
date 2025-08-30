#pragma once

namespace StudyEngine {
    enum class InputEvents
    {
        Key_W = 87, Key_A = 65, Key_S = 83, Key_D = 68, Key_Q = 81, Key_E = 69, Key_F = 70, Key_UP = 265, Key_DOWN = 264,
        Key_RIGHT = 262, Key_LEFT = 263, Key_ESCAPE = 256,

        MOUSE_LEFT = 1000, MOUSE_RIGHT = 1001, MOUSE_MIDDLE = 1002,
        MOUSE_MOVE_X = 1100, MOUSE_MOVE_Y = 1101,

        SCROLL_UP = 1200, SCROLL_DOWN = 1201,

        UNKNOWN = 0
    };

    enum class InputEventType {
        KeyPressed, KeyReleased,
        MousePressed, MouseReleased, MouseMoved,
        MouseScrolled
    };

    struct InputEvent {
        InputEventType type;
        union {
            struct { int keyCode; } key;
            struct { int button; float x, y; } mouse;
            struct { float deltaX, deltaY; } scroll;
            struct { float x, y; } mouseMove;
        };
        float x, y;
    };
}