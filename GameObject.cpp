#include "GameObject.h"
#include <unordered_map>
#include <chrono>
using namespace std;

GameObject::GameObject(Mesh* mesh, VkPipeline* pipeline, VkPipelineLayout* pipelineLayout, VkDescriptorSet* descriptorSet)
    : m_mesh(mesh)
    , m_pipeline(pipeline)
    , m_pipelineLayout(pipelineLayout)
    , m_descriptorSet(descriptorSet)
{
    // Initialize transform to identity matrix
    m_data.m_transform = glm::mat4(1.0f);
}

GameObject::~GameObject()
{
    // Note: Don't delete m_mesh, m_pipeline, etc. here as they're managed elsewhere
}

void GameObject::LoadObject(Mesh* mesh)
{
    m_mesh = mesh;
}

void GameObject::CreateDescriptorSet()
{
    // This would be implemented if you need per-object descriptor sets
    // For now, we'll use the global descriptor sets from VulkanControl
}

void GameObject::bindDescriptorSet(VkCommandBuffer cmd)
{
    // This would bind object-specific descriptor sets if needed
}

void GameObject::Draw(VkCommandBuffer cmd, int frameIdx)
{
    if (!m_mesh || !m_pipeline) {
        return; // Safety check
    }
    
    // Bind the pipeline
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
    
    // Bind descriptor sets - we need to get the current frame's descriptor set
    // This assumes VulkanControl has a way to access the current frame's descriptor set
    // You might need to modify this based on how your VulkanControl exposes descriptor sets
    
    // For now, we'll assume the descriptor set is passed in the constructor
    // In a more robust system, you'd get it from VulkanControl based on currentFrame
    if (m_descriptorSet) {
        vkCmdBindDescriptorSets(
            cmd,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            *m_pipelineLayout,
            0, 1, m_descriptorSet,  // Note: This uses the descriptor set passed in constructor
            0, nullptr
        );
    }
    
    // Draw the mesh
    m_mesh->Draw(cmd);
}
