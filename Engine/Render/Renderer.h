#pragma once
#include "stdafx.h"

namespace StudyEngine {
    class STUDY_ENGINE Renderer {
    public:
        Renderer();
        ~Renderer();
        static bool Init();

        static void BeginRender();

        static void EndRender();

        static void PreShutDown();
        static bool ShutDown();
    };
}
