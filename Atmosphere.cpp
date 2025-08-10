#include "stdafx.h"
#include "Atmosphere.h"

Atmosphere::Atmosphere():GameObject()
{
	CreateUniformBuffers();
	UpdateDescriptorSets();
}

Atmosphere::~Atmosphere()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(VulkanControl::Get()->GetDeviceContext(), atmosphereBuffer[i], nullptr);
		vkFreeMemory(VulkanControl::Get()->GetDeviceContext(), atmosphereBufferMemory[i], nullptr);

		vkDestroyBuffer(VulkanControl::Get()->GetDeviceContext(), sunBuffer[i], nullptr);
		vkFreeMemory(VulkanControl::Get()->GetDeviceContext(), sunBufferMemory[i], nullptr);
	}
}

void Atmosphere::Update(float deltaTime)
{

}

void Atmosphere::UpdateSun(float deltaSun) {
	sunAngle += deltaSun;
	if(sunAngle > Math::Pi){
		sunAngle = std::fmod(sunAngle,Math::Pi);
	} else if(sunAngle < -Math::Pi / 6.0f){
		sunAngle = -Math::Pi / 6.0f;
	}
	m_sunBuffer.sunDir.y = sinf(sunAngle);
	m_sunBuffer.sunDir.z = -cosf(sunAngle);
	m_sunBuffer.sunDir.Normalize();
}

void Atmosphere::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	memcpy(sunBufferMapped[currentFrame], &m_sunBuffer, sizeof(m_sunBuffer));
	GameObject::Draw(commandBuffer, currentFrame);
}

void Atmosphere::UpdateDescriptorSets()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorSet descriptorSet = VulkanControl::Get()->getDescriptorSet(i);

		VkDescriptorBufferInfo atmosphereInfo{};
		atmosphereInfo.buffer = atmosphereBuffer[i];
		atmosphereInfo.offset = 0;
		atmosphereInfo.range = sizeof(AtmosphereBuffer);

		VkDescriptorBufferInfo sunInfo{};
		sunInfo.buffer = sunBuffer[i];
		sunInfo.offset = 0;
		sunInfo.range = sizeof(SunBuffer);

		std::vector<VkWriteDescriptorSet> descriptorWrites(2);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSet;
		descriptorWrites[0].dstBinding = 1;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &atmosphereInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSet;
		descriptorWrites[1].dstBinding = 2;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &sunInfo;

		vkUpdateDescriptorSets(VulkanControl::Get()->GetDeviceContext(), 2, descriptorWrites.data(), 0, nullptr);
	}
}

void Atmosphere::CreateUniformBuffers()
{
	VkDeviceSize atmosphereSize = sizeof(AtmosphereBuffer);
	VkDeviceSize sunSize = sizeof(SunBuffer);

	atmosphereBuffer.resize(MAX_FRAMES_IN_FLIGHT);
	atmosphereBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
	atmosphereBufferMapped.resize(MAX_FRAMES_IN_FLIGHT);

	sunBuffer.resize(MAX_FRAMES_IN_FLIGHT);
	sunBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
	sunBufferMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VulkanControl::Get()->createBuffer(sunSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			sunBuffer[i],
			sunBufferMemory[i]);
		vkMapMemory(VulkanControl::Get()->GetDeviceContext(), sunBufferMemory[i], 0, sunSize, 0, &sunBufferMapped[i]);

		VulkanControl::Get()->createBuffer(atmosphereSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			atmosphereBuffer[i],
			atmosphereBufferMemory[i]);
		vkMapMemory(VulkanControl::Get()->GetDeviceContext(), atmosphereBufferMemory[i], 0, atmosphereSize, 0, &atmosphereBufferMapped[i]);
		memcpy(atmosphereBufferMapped[i], &m_atmosphereBuffer, sizeof(m_atmosphereBuffer));
	}
}
