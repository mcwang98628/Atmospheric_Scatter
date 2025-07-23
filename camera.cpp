#include "stdafx.h"
#include "camera.h"
#include "EventManager.h"

Camera::Camera()
{
	VkDeviceSize cameraSize = sizeof(CameraBuffer);
	descriptorWrites.resize(MAX_FRAMES_IN_FLIGHT);

	cameraBuffer.resize(MAX_FRAMES_IN_FLIGHT);
	cameraBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
	cameraBufferMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VulkanControl::Get()->createBuffer(cameraSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			cameraBuffer[i],
			cameraBufferMemory[i]);
		vkMapMemory(VulkanControl::Get()->GetDeviceContext(), cameraBufferMemory[i], 0, cameraSize, 0, &cameraBufferMapped[i]);
	}
	updateDescriptorSets();
	createCamera();

}

Camera::~Camera()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(VulkanControl::Get()->GetDeviceContext(), cameraBuffer[i], nullptr);
		vkFreeMemory(VulkanControl::Get()->GetDeviceContext(), cameraBufferMemory[i], nullptr);
	}
}

void Camera::moveForward(float velocity) {
	pos += velocity * view;
}

void Camera::moveHorizontal(float velocity) {
	pos -=  velocity * right;
}

void Camera::moveVertical(float velocity) {
	pos += velocity * up;
}

void Camera::UpdateLookAt(float xoffset, float yoffset)
{
	yaw += xoffset;
	pitch += yoffset;
	
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
	std::cout << "yaw: " << yaw << " pitch: " << pitch << std::endl;
}

void Camera::UpdateFov(float yoffset)
{
	fov -= yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	view = glm::normalize(front);
	
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(view, worldUp));
	up    = glm::normalize(glm::cross(right, view));
}

void Camera::updateCameraBuffer(uint32_t currentImage)
{
	cameraData.transform = glm::mat4(1.0f);
	glm::mat4 viewMat = glm::lookAt(pos, view + pos, up);
	glm::mat4 projection = glm::perspective(fov, WIDTH / (float)HEIGHT, near, far);
	projection[1][1] *= -1;  // Flip Y axis for Vulkan coordinate system

	cameraData.viewProjection = projection * viewMat;  // Flip Y axis for Vulkan coordinate system


	memcpy(cameraBufferMapped[currentImage], &cameraData, sizeof(cameraData));
}


void Camera::updateDescriptorSets()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorSet descriptorSet = VulkanControl::Get()->getDescriptorSet(i);
		
		VkDescriptorBufferInfo camInfo{};
		camInfo.buffer = cameraBuffer[i];
		camInfo.offset = 0;
		camInfo.range = sizeof(CameraBuffer);

		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = descriptorSet;
		descriptorWrites[i].dstBinding = 0;
		descriptorWrites[i].dstArrayElement = 0;
		descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[i].descriptorCount = 1;
		descriptorWrites[i].pBufferInfo = &camInfo;

		vkUpdateDescriptorSets(VulkanControl::Get()->GetDeviceContext(), 1, &descriptorWrites[i], 0, nullptr);
	}
}

VkDescriptorSetLayoutBinding Camera::getCameraDescriptorLayoutBinding()
{
	VkDescriptorSetLayoutBinding cameraBinding{};
	cameraBinding.binding            = 0;
	cameraBinding.descriptorCount    = 1;
	cameraBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	cameraBinding.pImmutableSamplers = nullptr;
	cameraBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	
	return cameraBinding;
}

void Camera::ProcessInput(double xposIn, double yposIn)
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

	// ProcessInput(xoffset, yoffset);
}


void Camera::createCamera() {
    pos = glm::vec3(0.0f, 1.f, 0.f);  // Position camera on surface of green planet (radius 50 + 2 units above)
    // rawCamera->pos = glm::vec3(0.0f, 6360.f, 30.f);

    // Try looking UPWARD instead (in case Vulkan coordinates are flipped)
    view = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));  // Look forward and UP
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    right = glm::cross(view, up);
    speed = 0;
    fov = 45.f;
    near = 0.01;
    far = 2200.f;
    yaw = 0.f;
    pitch = 0.f;
}

