#pragma once 

namespace StudyEngine{
    class VulkanBuffer {
        public:
            enum type{
                VERTEX,
                INDEX,
                UNIFORM
            };
            VulkanBuffer(const void* data, size_t dataSize, type usage);
            ~VulkanBuffer();
            void* GetBufferHandle() const;
            void BindBuffer();

        private:
            struct Impl;
            std::unique_ptr<Impl> m_impl;
            type m_bufferType;
        };
};