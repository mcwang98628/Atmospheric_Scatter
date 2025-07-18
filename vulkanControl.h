#pragma once

#include "stdafx.h"
#include "Atmosphere.h"

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

    std::vector<VkBuffer>        atmosphereBuffer;
    std::vector<VkDeviceMemory>  atmosphereBufferMemory;
    std::vector<void*>           atmosphereBufferMapped;

    std::vector<VkBuffer> sunBuffer;
    std::vector<VkDeviceMemory> sunBufferMemory;
    std::vector<void*> sunBufferMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    Sun* sun;
    Atmosphere* atmosphere;

    SunBuffer sunData;
    AtmosphereBuffer atmosphereData;

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
    void createGraphicsPipeline(std::string vertShaderPath, std::string fragShaderPath, VkPipeline& targetPipeline);
    void createCommandPool();
    void createDepthResources();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void createFramebuffers();
    void createTextureImage(std::string texturePath);
    void createTextureImageView();
    void createTextureSampler();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();
    void updateUniformBuffer(uint32_t currentImage);
    void beginRenderPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void endRenderPass(VkCommandBuffer commandBuffer);
    void recreateSwapChain(GLFWwindow* window);
    void CreateSun(Sun* Sun);
    void CreateAtmosphere(Atmosphere* rawAtmosphere);
    VkDescriptorSet getDescriptorSet(uint32_t frameIndex);
    VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
    void cleanUp();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void SetViewportAndScissors(VkCommandBuffer commandBuffer);


    ~VulkanControl();

private:

    static VulkanControl* m_graphics;

    VkDebugUtilsMessengerEXT debugMessenger;
    GLFWwindow* curWindow;

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
    void executeDrawCommand(VkCommandBuffer commandBuffer, VkPipeline pipeline, VkBuffer vBuffer, VkBuffer iBuffer, std::vector<uint32_t> indices);

};