#pragma once
#include "VertexFormat.h"

struct VkVertexInputBindingDescription;

struct VkVertexInputAttributeDescription;

class VulkanVertexHelper {
    public:
        static VkVertexInputBindingDescription getBindingDescription(); 
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
        
    };