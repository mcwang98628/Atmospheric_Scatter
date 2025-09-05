#pragma once
#include "stdafx.h"
#include "Vulkan/VulkanDescriptorSet.h"

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

        static VulkanDescriptorSet* GetCurrentDescriptorSet();
    private:
        static VulkanDescriptorSet* m_descriptorSets;
    };
}
