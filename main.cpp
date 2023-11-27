#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <chrono>
#include <array>

#include "windowControl.h"
#include "vulkanControl.h"


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::string MODEL_PATH = "models/terrain.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    WindowControl* windowController;
    VulkanControl* vulkanController;

    std::vector<Vertex> vertices;
    std::vector<Vertex> skyVertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> skyIndecis;


    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    void initWindow() {
        windowController = new WindowControl(this);
        window = windowController->window;
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
    void initSky() {
        Vertex v1;
        Vertex v2;
        Vertex v3;
        Vertex v4;
        v1.pos = { -1.0f, -1.0f, 0.0f };
        v2.pos = { 1.0f, -1.0f, 0.0f };
        v3.pos = { -1.0f, 1.0f, 0.0f };
        v4.pos = { 1.0f, 1.0f, 0.0f };

        skyVertices.push_back(v1);
        skyVertices.push_back(v2);
        skyVertices.push_back(v3);
        skyVertices.push_back(v4);
        skyIndecis.push_back(0);
        skyIndecis.push_back(1);
        skyIndecis.push_back(2);
        skyIndecis.push_back(3);


    }

    void initVulkan() {
        vulkanController = new VulkanControl();
        vulkanController->createInstance();
        initSky();
        vulkanController->setupDebugMessenger();
        vulkanController->createSurface(window);
        vulkanController->pickPhysicalDevice();
        vulkanController->createLogicalDevice();
        vulkanController->createSwapChain();
        vulkanController->createImageViews();
        vulkanController->createRenderPass();
        vulkanController->createDescriptorSetLayout();
        //vulkanController->createGraphicsPipeline("shaders/vert.spv", "shaders/frag.spv", true);
        vulkanController->createGraphicsPipeline("shaders/sky.vert.spv", "shaders/sky.frag.spv", false);
        vulkanController->createCommandPool();
        vulkanController->createDepthResources();
        vulkanController->createFramebuffers();
        vulkanController->createTextureImage(TEXTURE_PATH);
        vulkanController->createTextureImageView();
        vulkanController->createTextureSampler();
        //loadModel(MODEL_PATH);
        vulkanController->createCamera(window);
        vulkanController->createVertexBuffer(skyVertices);
        //vulkanController->createVertexBuffer(vertices);

        vulkanController->createIndexBuffer(skyIndecis);
        //drawSky();
        vulkanController->createUniformBuffers();
        vulkanController->createDescriptorPool();
        vulkanController->createDescriptorSets();
        vulkanController->createCommandBuffers();
        vulkanController->createSyncObjects();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(vulkanController->device);
    }

    void cleanup() {
        vulkanController->cleanUp();
        if (windowController != NULL && window != NULL) {
            windowController->destroyWindow(window);
        }

        glfwTerminate();
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void loadModel(std::string modelPath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    void drawFrame() {
        vkWaitForFences(vulkanController->device, 1, &vulkanController->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(vulkanController->device, vulkanController->swapChain, UINT64_MAX, vulkanController->imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            vulkanController->recreateSwapChain(window);
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vulkanController->updateUniformBuffer(currentFrame);

        vkResetFences(vulkanController->device, 1, &vulkanController->inFlightFences[currentFrame]);

        vkResetCommandBuffer(vulkanController->commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
        vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, indices);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { vulkanController->imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vulkanController->commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = { vulkanController->renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vulkanController->graphicsQueue, 1, &submitInfo, vulkanController->inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { vulkanController->swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(vulkanController->presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            vulkanController->recreateSwapChain(window);
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}