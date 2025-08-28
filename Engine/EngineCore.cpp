#include "stdafx.h"
#include "EngineCore.h"
#include "Application/WinApplication.h"
#include <vulkanControl.h>
#include <Input\InputManager.h>

const std::string TEXTURE_PATH = "textures/viking_room.png";


bool EngineCore::Initialize(void* gamePtr)
{
    WinApplication::Init(gamePtr);

    VulkanControl* vulkanController = VulkanControl::Get();

    vulkanController->createInstance();
    vulkanController->setupDebugMessenger();
    vulkanController->createSurface(WinApplication::GetWindow());
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
    InputManager::Instance()->Init();

    vulkanController->createCommandBuffers();
    vulkanController->createSyncObjects();

    return true;
}

void EngineCore::Update()
{
    WinApplication::Get()->Update();

}

void EngineCore::BeginFrame()
{
    VulkanControl* vulkanController = VulkanControl::Get();
    vulkanController->BeginRenderer();
}

void EngineCore::Render()
{
}

void EngineCore::EndFrame()
{
}

void EngineCore::Shutdown()
{
}
