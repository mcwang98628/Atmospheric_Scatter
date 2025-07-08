#pragma once

#include "vulkanControl.h"

class GameObject {
    public:
        GameObject();
        ~GameObject();

        void Update(float deltaTime);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        VkDeviceMemory vertexBufferMemory1;
        VkDeviceMemory indexBufferMemory1;
        VkBuffer vertexBuffer1, indexBuffer1;


};