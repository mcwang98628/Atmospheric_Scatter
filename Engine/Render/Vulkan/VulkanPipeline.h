#pragma once 
#include "stdafx.h"
using namespace std;

typedef struct VkCommandBuffer_T* VkCommandBuffer;

namespace StudyEngine {
    class VulkanPipeline {
        public:
            VulkanPipeline(std::string vertShaderPath, std::string fragShaderPath);
            ~VulkanPipeline();

            void BindCommandBuffer(VkCommandBuffer cmd);
        private:
            struct Impl;
            std::unique_ptr<Impl> m_impl;
    };
};