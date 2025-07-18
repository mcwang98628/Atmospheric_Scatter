#pragma once

#include "vulkanControl.h"

class GameObject {
    public:
        ~GameObject();

        void Update(float deltaTime);
        void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

        void LoadModel(std::string modelPath);
        void CreateVertexBuffer();
        void CreateIndexBuffer();


       /* virtual void CreateUniformBuffers();*/
       void UpdateDescriptorSets();
       void BindGraphicPipeline(std::string vertShaderPath, std::string fragShaderPath);

       struct RenderData {
            glm::mat4 m_transform;
        };

    private:
        VkPipeline m_pipeline;
        VkDeviceMemory m_vertexBufferMemory;
        VkDeviceMemory m_indexBufferMemory;
        VkBuffer m_vertexBuffer, m_indexBuffer;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

};