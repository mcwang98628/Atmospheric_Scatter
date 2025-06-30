#include "Game.h"

Game::Game() : m_vulkanController(nullptr), m_planeObject(nullptr), m_skyObject(nullptr) {
}

Game::~Game() {
    ShutDown();
}

void Game::Init(GLFWwindow* hwd, int width, int height) {
    window = hwd;
    
    // Initialize Vulkan
    m_vulkanController = new VulkanControl();
    m_vulkanController->Init(hwd, width, height);
    
    // Load meshes
    m_planeMesh = std::make_unique<Mesh>();
    m_skyMesh = std::make_unique<Mesh>();
    
    if (!m_planeMesh->LoadMesh(TERRAIN_PATH)) {
        throw std::runtime_error("Failed to load plane mesh!");
    }
    
    if (!m_skyMesh->LoadMesh(SKY_PATH)) {
        throw std::runtime_error("Failed to load sky mesh!");
    }
    
    // Create game objects
    m_planeObject = new GameObject(
        m_planeMesh.get(), 
        &m_vulkanController->graphicsPipeline1, 
        &m_vulkanController->pipelineLayout, 
        &m_vulkanController->descriptorSets[0]
    );
    
    m_skyObject = new GameObject(
        m_skyMesh.get(), 
        &m_vulkanController->graphicsPipeline2, 
        &m_vulkanController->pipelineLayout, 
        &m_vulkanController->descriptorSets[0]
    );
    
    // Set transforms
    m_planeObject->SetTransform(glm::mat4(1.0f)); // Identity for now
    m_skyObject->SetTransform(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f))); // Scale sky sphere
    
    // Add to game objects list
    m_gameObjects.push_back(std::unique_ptr<GameObject>(m_planeObject));
    m_gameObjects.push_back(std::unique_ptr<GameObject>(m_skyObject));
}

void Game::ProcessEvents() {
    // Handle input events here
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Game::Update(float deltaTime) {
    // Update game logic here
    // You can update object transforms, camera, etc.
}

void Game::RenderFrame() {
    vkWaitForFences(m_vulkanController->device, 1, &m_vulkanController->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(m_vulkanController->device, 1, &m_vulkanController->inFlightFences[currentFrame]);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_vulkanController->device, m_vulkanController->swapChain, UINT64_MAX, m_vulkanController->imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_vulkanController->recreateSwapChain(window);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    m_vulkanController->updateUniformBuffer(currentFrame);

    vkResetCommandBuffer(m_vulkanController->commandBuffers[currentFrame], 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_vulkanController->commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    
    m_vulkanController->beginRenderPass(m_vulkanController->commandBuffers[currentFrame], imageIndex);
    
    // Draw sky first (background)
    m_skyObject->Draw(m_vulkanController->commandBuffers[currentFrame], currentFrame);
    
    // Draw plane (foreground)
    m_planeObject->Draw(m_vulkanController->commandBuffers[currentFrame], currentFrame);
    
    m_vulkanController->endRenderPass(m_vulkanController->commandBuffers[currentFrame]);
    
    if (vkEndCommandBuffer(m_vulkanController->commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_vulkanController->imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_vulkanController->commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { m_vulkanController->renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_vulkanController->graphicsQueue, 1, &submitInfo, m_vulkanController->inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_vulkanController->swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(m_vulkanController->presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        m_vulkanController->recreateSwapChain(window);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Game::ShutDown() {
    if (m_vulkanController) {
        m_vulkanController->cleanUp();
        delete m_vulkanController;
        m_vulkanController = nullptr;
    }
}


