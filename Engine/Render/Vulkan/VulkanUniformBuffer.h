#pragma once
#include "stdafx.h"

typedef struct VkBuffer_T* VkBuffer;

namespace StudyEngine {
    class VulkanUniformBuffer {
        public:
            VulkanUniformBuffer(size_t dataSize);
            void UpdateUniformData(void* data, size_t dataSize);
            VkBuffer GetBufferHandle() const;
            ~VulkanUniformBuffer();
        private:
            struct Impl;
            std::unique_ptr<Impl> m_impl;
        };
}
