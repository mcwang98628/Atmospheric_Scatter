#pragma once

#include "Render/Vulkan/VertexFormat.h"
#include "Render/Vulkan/VulkanBuffer.h"

typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef struct VkDescriptorSet_T* VkDescriptorSet;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkPipeline_T* VkPipeline;


namespace StudyEngine {
    class STUDY_ENGINE GameObject {
    public:
        GameObject();
        ~GameObject();

        virtual void Update(float deltaTime);
        virtual void Draw();

        void LoadModel(std::string modelPath);

        /* virtual void CreateUniformBuffers();*/
        virtual void UpdateDescriptorSets();
        virtual void BindGraphicPipeline(std::string vertShaderPath, std::string fragShaderPath);
        virtual void CreateUniformBuffers();
        virtual void DrawIndexed();



        struct RenderData {
            Matrix4 m_transform;
        };

    private:
        VkPipeline m_pipeline;

        VulkanBuffer* m_vertexBuffer = nullptr;
        VulkanBuffer* m_indexBuffer = nullptr;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

    };
}