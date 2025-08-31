#pragma once
#include "stdafx.h"

class EngineCore {
public:
    static bool Initialize(void* gamePtr);
    static void Update();

    static void BeginFrame();
    static void Render();
    static void EndFrame();

    static void Shutdown();
};