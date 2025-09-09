#include "stdafx.h"
#include "Atmosphere.h"
#include "Render/Vulkan/vulkanControl.h"
#include "Render/Renderer.h"

namespace StudyEngine {
	Atmosphere::Atmosphere() : GameObject()
	{
		CreateUniformBuffers();
		UpdateDescriptorSets();
	}

	Atmosphere::~Atmosphere()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			delete atmosphereBuffer[i];
			delete sunBuffer[i];
		}
	}

	void Atmosphere::Update(float deltaTime)
	{

	}

	void Atmosphere::UpdateSun(float deltaSun) {
		sunAngle += deltaSun;
		if (sunAngle > Math::Pi) {
			sunAngle = std::fmod(sunAngle, Math::Pi);
		}
		else if (sunAngle < -Math::Pi / 6.0f) {
			sunAngle = -Math::Pi / 6.0f;
		}
		m_sunBuffer.sunDir.y = sinf(sunAngle);
		m_sunBuffer.sunDir.z = -cosf(sunAngle);
		m_sunBuffer.sunDir.Normalize();
	}

	void Atmosphere::Draw()
	{
		VkCommandBuffer cmd = VulkanControl::Get()->GetCommandBuffer();
		uint32_t frameInd = VulkanControl::Get()->GetCurrentFrameIndex();
		for (VulkanUniformBuffer* each : sunBuffer)
		{
			each->UpdateUniformData(&m_sunBuffer, sizeof(m_sunBuffer));
		}
		GameObject::Draw();
	}

	void Atmosphere::UpdateDescriptorSets()
	{
		VulkanDescriptorSet* m_currentDescriptorSet = Renderer::GetCurrentDescriptorSet();
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			m_currentDescriptorSet->UpdateDescriptorSet(sunBuffer[i], 2, sizeof(SunBuffer), i);
			m_currentDescriptorSet->UpdateDescriptorSet(atmosphereBuffer[i], 1, sizeof(AtmosphereBuffer), i);
		}
	}

	void Atmosphere::CreateUniformBuffers()
	{
		VkDeviceSize atmosphereSize = sizeof(AtmosphereBuffer);
		VkDeviceSize sunSize = sizeof(SunBuffer);

		atmosphereBuffer.resize(MAX_FRAMES_IN_FLIGHT);

		sunBuffer.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			atmosphereBuffer[i] = new VulkanUniformBuffer(atmosphereSize);
			sunBuffer[i] = new VulkanUniformBuffer(sunSize);
			atmosphereBuffer[i]->UpdateUniformData(&m_atmosphereBuffer, sizeof(m_atmosphereBuffer));
		}
	}
}