#pragma once

// Standard C++ includes
#include <array>
#include <vector>
#include <optional>
#include <cstddef>  // for offsetof
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <set>
#include <unordered_map>
#include <string>
#include <fstream>

// Vulkan (must come before GLFW)
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define MAX_FRAMES_IN_FLIGHT 2


#define WIDTH 2560.f
#define HEIGHT 1440.f

#define MOUSE_SENSITIVITY 0.1f
#define ENABLE_MOUSE_CALLBACK true


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME
};

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
typedef struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
} QueueFamilyIndices;

struct SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct AtmosphereBuffer {
    alignas(4) int viewSamples = 16;
    alignas(4) int lightSamples = 8;
    alignas(16) glm::vec3 sunIntensity = glm::vec3(20.f, 20.f, 20.f);    // Intensity of the sun

    alignas(16) glm::vec3 scatterRayleigh = glm::vec3(5.802f, 13.558f, 33.1f);
    alignas(4) float scatterMie = 3.996f;
    alignas(4) float absorbMie = 4.4f;
    alignas(4) float planetRadius = 6360;
    alignas(4) float atmosphereRadius = 6460;
    alignas(4) float rayleighScaleHeight = 7.994f;
    alignas(4) float mieScaleHeight = 1.2f;
    alignas(4) float anisotropy = 0.888f;  //  - anisotropy of the medium
};

struct SunBuffer {
    alignas(16) glm::vec3  sunPos;    // Position of the sun, light direction
};
