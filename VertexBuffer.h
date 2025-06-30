#pragma once 
#include <glm/glm.hpp>
#include "vulkanControl.h"
#include "helper.h"

class VertexBuffer
{ 
public:
	VertexBuffer(std::vector<Vertex> verts, std::vector<uint32_t> index);
    ~VertexBuffer();
    void Draw(VkCommandBuffer cmd) const;

    VkBuffer vertexBuffer;

    VkBuffer indexBuffer;
    size_t indexCount;

    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;

private:
	
};