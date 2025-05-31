#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/gtx/hash.hpp>
#include <optional>

#define MAX_FRAMES_IN_FLIGHT 2

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

typedef struct SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
} SwapChainSupportDetails;

typedef struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
    }
} Vertex;

typedef struct CameraBuffer {
    alignas(16) glm::mat4 transform;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
} CameraBuffer;

typedef struct AtmosphereBuffer {
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
} AtmosphereBuffer;

typedef struct SunBuffer {
    alignas(16) glm::vec3  sunPos;    // Position of the sun, light direction
} SunBuffer;
