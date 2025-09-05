#include "stdafx.h"
#include "Renderer.h"
#include "Vulkan/vulkanControl.h"
#include "Vulkan/VulkanDescriptorSet.h"


namespace StudyEngine {
    VulkanDescriptorSet* Renderer::m_descriptorSets = nullptr;
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

        vulkanController->createCommandPool();
        vulkanController->createDepthResources();
        vulkanController->createFramebuffers();
        vulkanController->createTextureImage(TEXTURE_PATH);
        vulkanController->createTextureImageView();
        vulkanController->createTextureSampler();

        vulkanController->createDescriptorPool();

        m_descriptorSets = new VulkanDescriptorSet();

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

    void Renderer::PreShutDown()
    {
        VulkanControl::Get()->WaitIdle();
    }

    bool Renderer::ShutDown()
	{
        VulkanControl::Get()->cleanUp();
		return false;
	}

    VulkanDescriptorSet* Renderer::GetCurrentDescriptorSet()
    {
        return m_descriptorSets;
    }


};


