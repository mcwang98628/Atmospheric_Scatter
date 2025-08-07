#include "stdafx.h"
#include "Game.h"

#include "windowControl.h"

// settings
const std::string TERRAIN_PATH = "models/Plane.obj";
const std::string SKY_PATH = "models/sphere.obj";

const std::string TEXTURE_PATH = "textures/viking_room.png";

Camera* Game::camera = new Camera();


// Sun
//Sun sun = {};
//void UpdateSun(float delta);

// Atmosphere
//Atmosphere atmosphere = {};

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//void UpdateSun(float delta)
//{
//    sun.sunAngle = glm::mod(sun.sunAngle + 0.5 * delta, 3.1415926);
//    sun.sunDir.y = glm::sin(sun.sunAngle);
//    sun.sunDir.z = -glm::cos(sun.sunAngle);
//}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    
}


Game::Game()
{
}

Game::~Game()
{
}

bool Game::Init()
{
    initWindow();
    initVulkan();
    return true;
}

void Game::initWindow()
{
    WindowControl::Init(this);
    glfwSetFramebufferSizeCallback(WindowControl::GetWindow(), framebufferResizeCallback);
    glfwSetScrollCallback(WindowControl::GetWindow(), scroll_callback);

    glfwSetInputMode(WindowControl::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(WindowControl::GetWindow(), ProcessMouseInput);
}

void Game::initVulkan()
{
    VulkanControl* vulkanController = VulkanControl::Get();

    vulkanController->createInstance();
    vulkanController->setupDebugMessenger();
    vulkanController->createSurface(WindowControl::GetWindow());
    vulkanController->pickPhysicalDevice();
    vulkanController->createLogicalDevice();
    vulkanController->createSwapChain();
    vulkanController->createImageViews();
    vulkanController->createRenderPass();
    vulkanController->createDescriptorSetLayout();
    vulkanController->createPipelineLayout();
    //vulkanController->createGraphicsPipeline("shaders/terrainVert.spv", "shaders/terrainFrag.spv", vulkanController->graphicsPipeline1);
    //vulkanController->createGraphicsPipeline("shaders/skyVert.spv", "shaders/skyFrag.spv", vulkanController->graphicsPipeline2);
    vulkanController->createCommandPool();
    vulkanController->createDepthResources();
    vulkanController->createFramebuffers();
    vulkanController->createTextureImage(TEXTURE_PATH);
    vulkanController->createTextureImageView();
    vulkanController->createTextureSampler();

    vulkanController->createDescriptorPool();
    vulkanController->createDescriptorSets();

    terrain = new GameObject();
    sky = new Atmosphere();
    terrain->LoadModel(TERRAIN_PATH);
    sky->LoadModel(SKY_PATH);
    terrain->BindGraphicPipeline("shaders/terrainVert.spv", "shaders/terrainFrag.spv");
    sky->BindGraphicPipeline("shaders/skyVert.spv", "shaders/skyFrag.spv");

    // vulkanController->CreateSun(&sun);
    terrain->CreateVertexBuffer();
    terrain->CreateIndexBuffer();

    sky->CreateVertexBuffer();
    sky->CreateIndexBuffer();

    // Initialize camera descriptor sets after VulkanControl creates the main descriptor sets
    camera->Init();
    
    vulkanController->createCommandBuffers();
    vulkanController->createSyncObjects();
}

void Game::Update()
{

    const auto currTime = static_cast<float>(glfwGetTime());
    deltaTime = currTime - lastFrame;
    lastFrame = currTime;

    glfwPollEvents();
    camera->updateCameraBuffer(currentFrame);

    //drawFrame();
    //processInput();

    //UpdateSun(deltaTime);

    vkDeviceWaitIdle(VulkanControl::Get()->GetDeviceContext());
}

void Game::ShutDown()
{
    vkDeviceWaitIdle(VulkanControl::Get()->GetDeviceContext());
    delete camera;
    VulkanControl* vulkanController = VulkanControl::Get();
    WindowControl* windowController = WindowControl::Get();
    delete terrain;
    delete sky;
    vulkanController->cleanUp();
    if (windowController != nullptr && WindowControl::GetWindow() != nullptr) {
        windowController->destroyWindow(WindowControl::GetWindow());
    }

    glfwTerminate();
}

void Game::DrawFrame()
{
    VulkanControl* vulkanController = VulkanControl::Get();

    vkWaitForFences(vulkanController->device, 1, &vulkanController->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(vulkanController->device, vulkanController->swapChain, UINT64_MAX, vulkanController->imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        vulkanController->recreateSwapChain(WindowControl::GetWindow());
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vulkanController->updateUniformBuffer(currentFrame);

    vkResetFences(vulkanController->device, 1, &vulkanController->inFlightFences[currentFrame]);

    vkResetCommandBuffer(vulkanController->commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(vulkanController->commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    vulkanController->beginRenderPass(vulkanController->commandBuffers[currentFrame], imageIndex);
    vulkanController->SetViewportAndScissors(vulkanController->commandBuffers[currentFrame]);
    sky->Draw(vulkanController->commandBuffers[currentFrame], currentFrame);
    terrain->Draw(vulkanController->commandBuffers[currentFrame], currentFrame);
    // vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, vulkanController->graphicsPipeline2, vulkanController->vertexBuffer2, vulkanController->indexBuffer2, indices2);  // Draw blue sky first (background)
    // vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, vulkanController->graphicsPipeline1, vulkanController->vertexBuffer1, vulkanController->indexBuffer1, indices1);  // Draw green ground second (foreground)
    vulkanController->endRenderPass(vulkanController->commandBuffers[currentFrame]);
    if (vkEndCommandBuffer(vulkanController->commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { vulkanController->imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vulkanController->commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { vulkanController->renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(vulkanController->graphicsQueue, 1, &submitInfo, vulkanController->inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { vulkanController->swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(vulkanController->presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        vulkanController->recreateSwapChain(WindowControl::GetWindow());
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Game::ProcessInput()
{
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(WindowControl::GetWindow(), true);

    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (enableMouseCallback)
        {
            glfwSetInputMode(WindowControl::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            enableMouseCallback = false;
        }
        else {
            glfwSetInputMode(WindowControl::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            enableMouseCallback = true;
        }
    }

    float cameraSpeed = static_cast<float>(25 * deltaTime);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(cameraSpeed);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
        camera->moveForward(-cameraSpeed);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
        camera->moveHorizontal(-cameraSpeed);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
        camera->moveHorizontal(cameraSpeed);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
        camera->moveVertical(-cameraSpeed);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
        camera->moveVertical(cameraSpeed);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        sky->UpdateSun(0.01f);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        sky->UpdateSun(-.01f);
    if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_F) == GLFW_PRESS)
        camera->PrintCurrentCamMatrix();
}

void Game::ProcessMouseInput(GLFWwindow* win, double xposIn, double yposIn)
{
    if (ENABLE_MOUSE_CALLBACK)
        camera->ProcessInput(xposIn, yposIn);
}