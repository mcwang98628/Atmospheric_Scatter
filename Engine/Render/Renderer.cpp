#include "stdafx.h"
#include "Renderer.h"
#include "Vulkan/vulkanControl.h"


namespace StudyEngine {
    Renderer::Renderer()
    {
    }
    Renderer::~Renderer()
    {
    }
    bool Renderer::Init()
	{
        const std::string TEXTURE_PATH = "textures/viking_room.png";

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
        //InputManager::Instance()->Init();

        vulkanController->createCommandBuffers();
        vulkanController->createSyncObjects();

        return true;
	}

    void Renderer::BeginRender()
    {
        VulkanControl::Get()->BeginRenderer();
    }

    void Renderer::EndRender()
    {
        VulkanControl::Get()->EndRenderer();
    }

	bool Renderer::ShutDown()
	{
        VulkanControl::Get()->cleanUp();
		return false;
	}


};


