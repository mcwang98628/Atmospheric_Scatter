#pragma once
#include "stdafx.h"

namespace StudyEngine {
    class STUDY_ENGINE Renderer {
    public:
        Renderer();
        ~Renderer();
        static bool Init();

        static bool ShutDown();
    };
}
