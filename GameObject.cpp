#include "stdafx.h"
#include "GameObject.h"
#include "vulkanControl.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

GameObject::~GameObject()
{
	VkDevice device = VulkanControl::Get()->GetDeviceContext();
    vkDestroyPipeline(device, m_pipeline, nullptr);

    vkDestroyBuffer(device, m_vertexBuffer, nullptr);
    vkDestroyBuffer(device, m_indexBuffer, nullptr);
    vkFreeMemory(device, m_vertexBufferMemory, nullptr);
    vkFreeMemory(device, m_indexBufferMemory, nullptr);

}

void GameObject::Update(float deltaTime)
{

}

void GameObject::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
    VkDeviceSize offsets = 0;
	VkDescriptorSet descriptorSet = VulkanControl::Get()->getDescriptorSet(currentFrame);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanControl::Get()->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_vertexBuffer, &offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
}


void GameObject::LoadModel(std::string modelPath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
                m_vertices.push_back(vertex);
            }

            m_indices.push_back(uniqueVertices[vertex]);
        }
    }
    //std::cout << m_vertices.size();
}

void GameObject::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanControl::Get()->GetDeviceContext(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanControl::Get()->GetDeviceContext(), stagingBufferMemory);

    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

    VulkanControl::Get()->copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    vkDestroyBuffer(VulkanControl::Get()->GetDeviceContext(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanControl::Get()->GetDeviceContext(), stagingBufferMemory, nullptr);
}

void GameObject::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanControl::Get()->GetDeviceContext(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_indices.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanControl::Get()->GetDeviceContext(), stagingBufferMemory);

    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

    VulkanControl::Get()->copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    vkDestroyBuffer(VulkanControl::Get()->GetDeviceContext(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanControl::Get()->GetDeviceContext(), stagingBufferMemory, nullptr);
}

void GameObject::UpdateDescriptorSets()
{
    
}

void GameObject::BindGraphicPipeline(std::string vertShaderPath, std::string fragShaderPath)
{
    VulkanControl::Get()->createGraphicsPipeline(vertShaderPath, fragShaderPath, m_pipeline);
}