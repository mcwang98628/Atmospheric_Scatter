#pragma once

#include <vector>
#include <string>
#include <vulkan/vulkan.h>

class Mesh {
    public:
        Mesh(std::string modelPath);
        ~Mesh();
};