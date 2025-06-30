#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>

class GameObject
{
public:
    GameObject(Mesh* mesh, VkPipeline* pipeline, VkPipelineLayout* pipelineLayout, VkDescriptorSet* descriptorSet);
    ~GameObject();
    
    struct RenderData{
        glm::mat4 m_transform;
    };
    
    void LoadObject(Mesh* mesh);
    void CreateDescriptorSet();
    void bindDescriptorSet(VkCommandBuffer cmd);
    void Draw(VkCommandBuffer cmd, int frameIdx);
    
    // Getters
    Mesh* GetMesh() const { return m_mesh; }
    VkPipeline* GetPipeline() const { return m_pipeline; }
    VkPipelineLayout* GetPipelineLayout() const { return m_pipelineLayout; }
    VkDescriptorSet* GetDescriptorSet() const { return m_descriptorSet; }
    
    // Transform
    void SetTransform(const glm::mat4& transform) { m_data.m_transform = transform; }
    const glm::mat4& GetTransform() const { return m_data.m_transform; }

private:
    Mesh* m_mesh;
    VkPipeline* m_pipeline;
    VkPipelineLayout* m_pipelineLayout;
    VkDescriptorSet* m_descriptorSet;
    RenderData m_data;
};
