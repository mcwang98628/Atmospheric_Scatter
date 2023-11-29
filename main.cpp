#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <chrono>
#include <array>

#include "Atmosphere.h"
#include "windowControl.h"
#include "vulkanControl.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
#define MOUSE_SENSITIVITY 0.1f
const uint32_t WIDTH = 2560;
const uint32_t HEIGHT = 1440;

const std::string MODEL_PATH = "models/sphere.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";

// Camera
Camera camera = {};
bool firstMouse = true;
bool enableMouseCallback = true;
float lastX =  WIDTH / 2.0;
float lastY =  HEIGHT / 2.0;

// Sun
Sun sun ={};
void UpdateSun(float delta);

// Atmosphere
Atmosphere atmosphere ={};

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    GLFWwindow* window = nullptr;
    WindowControl* windowController = nullptr;
    VulkanControl* vulkanController = nullptr;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    void initWindow() {
        windowController = new WindowControl(this);
        window = windowController->window;
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void initVulkan() {
        vulkanController = new VulkanControl();
        vulkanController->createInstance();
        vulkanController->setupDebugMessenger();
        vulkanController->createSurface(window);
        vulkanController->pickPhysicalDevice();
        vulkanController->createLogicalDevice();
        vulkanController->createSwapChain();
        vulkanController->createImageViews();
        vulkanController->createRenderPass();
        vulkanController->createDescriptorSetLayout();
        vulkanController->createGraphicsPipeline("shaders/vert.spv", "shaders/frag.spv");
        vulkanController->createCommandPool();
        vulkanController->createDepthResources();
        vulkanController->createFramebuffers();
        vulkanController->createTextureImage(TEXTURE_PATH);
        vulkanController->createTextureImageView();
        vulkanController->createTextureSampler();
        loadModel(MODEL_PATH);
        vulkanController->createCamera(&camera);
        vulkanController->CreateSun(&sun);
        vulkanController->createVertexBuffer(vertices);
        vulkanController->createIndexBuffer(indices);
        vulkanController->createUniformBuffers();
        vulkanController->createDescriptorPool();
        vulkanController->createDescriptorSets();
        vulkanController->createCommandBuffers();
        vulkanController->createSyncObjects();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            const auto currTime = static_cast<float>(glfwGetTime());
            deltaTime = currTime - lastFrame;
            lastFrame = currTime;
            
            glfwPollEvents();
            drawFrame();
            processInput();
            //UpdateSun(deltaTime);
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

    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            if (enableMouseCallback)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                enableMouseCallback = false;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                enableMouseCallback = true;
            }
        }

        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.moveForward(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.moveForward(-cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.moveHorizontal(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.moveHorizontal(-cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.moveVertical(cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.moveVertical(-cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            UpdateSun(0.00005f);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            UpdateSun(-.00005f);
    }

};
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = (xpos - lastX) * MOUSE_SENSITIVITY;
    float yoffset = (lastY - ypos) * MOUSE_SENSITIVITY;
    lastX = xpos;
    lastY = ypos;
    
    if (enableMouseCallback)
        camera.UpdateLookAt(xoffset, yoffset);
}

void UpdateSun(float delta)
{
    sun.sunAngle = glm::mod(sun.sunAngle + 0.5 * delta, 3.1415926 + glm::radians(20.f));
    sun.sunDir.y = glm::sin(sun.sunAngle);
    sun.sunDir.z = -glm::cos(sun.sunAngle);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // if (enableMouseCallback)
    //     UpdateSun(static_cast<float>(yoffset));
}


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