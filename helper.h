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
    glm::vec3 color;
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
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
} Vertex;

typedef struct UniformBufferObject {
    alignas(16) glm::mat4 M;
    alignas(16) glm::mat4 MVP;

    alignas(16) glm::vec3 viewPos;   // Position of the viewer
    alignas(16) glm::vec3  sunPos;    // Position of the sun, light direction

    // Number of samples along the view ray and light ray
    alignas(4) int viewSamples;
    alignas(4) int lightSamples;

    alignas(4) float I_sun;    // Intensity of the sun
    alignas(4) float R_e;      // Radius of the planet [m]
    alignas(4) float R_a;      // Radius of the atmosphere [m]
    alignas(16) glm::vec3  beta_R;   // Rayleigh scattering coefficient
    alignas(4) float beta_M;   // Mie scattering coefficient
    alignas(4) float H_R;      // Rayleigh scale height
    alignas(4) float H_M;      // Mie scale height
    alignas(4) float g;        // Mie scattering direction - 
    //  - anisotropy of the medium

    alignas(4) float toneMappingFactor;    ///< Whether tone mapping is applied
} UniformBufferObject;