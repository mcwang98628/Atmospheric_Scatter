#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex> verts, std::vector<uint32_t> indices)
{
    VkDeviceSize bufferSize = sizeof(verts[0]) * verts.size();

    VkBuffer stagingVertBuffer;
    VkDeviceMemory stagingVertBufferMemory;
    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingVertBuffer, stagingVertBufferMemory);

    void* vertData;
    vkMapMemory(VulkanControl::Get()->GetDevice(), stagingVertBufferMemory, 0, bufferSize, 0, &vertData);
    memcpy(vertData, verts.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanControl::Get()->GetDevice(), stagingVertBufferMemory);

    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    VulkanControl::Get()->copyBuffer(stagingVertBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(VulkanControl::Get()->GetDevice(), stagingVertBuffer, nullptr);
    vkFreeMemory(VulkanControl::Get()->GetDevice(), stagingVertBufferMemory, nullptr);

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    indexCount = indices.size();
    VkBuffer stagingIndBuffer;
    VkDeviceMemory stagingIndBufferMemory;
    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingIndBuffer, stagingIndBufferMemory);

    void* indData;
    vkMapMemory(VulkanControl::Get()->GetDevice(), stagingIndBufferMemory, 0, bufferSize, 0, &indData);
    memcpy(indData, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanControl::Get()->GetDevice(), stagingIndBufferMemory);

    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    VulkanControl::Get()->copyBuffer(stagingIndBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(VulkanControl::Get()->GetDevice(), stagingIndBuffer, nullptr);
    vkFreeMemory(VulkanControl::Get()->GetDevice(), stagingIndBufferMemory, nullptr);
}

VertexBuffer::~VertexBuffer()
{
    vkDestroyBuffer(VulkanControl::Get()->GetDevice(), indexBuffer, nullptr);
    vkDestroyBuffer(VulkanControl::Get()->GetDevice(), vertexBuffer, nullptr);
    vkFreeMemory(VulkanControl::Get()->GetDevice(), vertexBufferMemory, nullptr);
    vkFreeMemory(VulkanControl::Get()->GetDevice(), indexBufferMemory, nullptr);

}

void VertexBuffer::Draw(VkCommandBuffer cmd) const
{
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer, offsets);
    vkCmdBindIndexBuffer(cmd, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, indexCount, 1, 0, 0, 0);

}
