#pragma once
#include "stdafx.h"
#include "Application/WinApplication.h"
#include <vulkan/vulkan.h>

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

namespace StudyEngine {
    class VulkanControl {
    public:
        static VulkanControl* Get() {
            if (m_graphics == nullptr) {
                m_graphics = new VulkanControl();
            }
            return m_graphics;
            
        }

        VkDevice GetDeviceContext() { return device; }

        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkRenderPass renderPass;

        VkDescriptorSetLayout descriptorSetLayout;

        VkPipelineLayout pipelineLayout;

        VkCommandPool commandPool;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;

        VkSampler textureSampler;

        VkDescriptorPool descriptorPool;

        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        VulkanControl();
        void createInstance();
        void setupDebugMessenger();
        void createSurface(GLFWwindow* window);
        VkPhysicalDevice pickPhysicalDevice();
        VkDevice createLogicalDevice();
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        VkSwapchainKHR createSwapChain();
        void createImageViews();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createRenderPass();
        void createDescriptorSetLayout();
        void createPipelineLayout();
        void createCommandPool();
        void createDepthResources();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void createFramebuffers();
        void createTextureImage(std::string texturePath);
        void createTextureImageView();
        void createTextureSampler();
        void createDescriptorPool();
        void createCommandBuffers();
        void createSyncObjects();
        void beginRenderPass(VkCommandBuffer commandBuffer);
        void endRenderPass(VkCommandBuffer commandBuffer);
        void recreateSwapChain(GLFWwindow* window);
        VkRenderPass getRenderPass() { return renderPass; }
        VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
        void cleanUp();

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void SetViewportAndScissors(VkCommandBuffer commandBuffer);

        void BeginRenderer();
        void EndRenderer();
        VkCommandBuffer GetCommandBuffer();
        uint32_t GetCurrentFrameIndex();
        void WaitIdle();

        ~VulkanControl();
    private:

        bool framebufferResized = false;
        uint32_t imageIndex;

        static VulkanControl* m_graphics;

        VkDebugUtilsMessengerEXT debugMessenger;

        uint32_t currentFrame;

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkFormat findDepthFormat();
        void cleanupSwapChain();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        void SubmitCommands();
        void PresentFrame();
    };
};
