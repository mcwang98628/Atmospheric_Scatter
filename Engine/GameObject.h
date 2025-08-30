#pragma once

#include "Render/Vulkan/VertexFormat.h"

typedef struct VkBuffer_T* VkBuffer;
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
        virtual void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

        void LoadModel(std::string modelPath);
        void CreateVertexBuffer();
        void CreateIndexBuffer();


        /* virtual void CreateUniformBuffers();*/
        virtual void UpdateDescriptorSets();
        virtual void BindGraphicPipeline(std::string vertShaderPath, std::string fragShaderPath);
        virtual void CreateUniformBuffers();

        struct RenderData {
            Matrix4 m_transform;
        };

    private:
        VkPipeline m_pipeline;
        VkDeviceMemory m_vertexBufferMemory;
        VkDeviceMemory m_indexBufferMemory;
        VkBuffer m_vertexBuffer, m_indexBuffer;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

    };
}