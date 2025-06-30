#define FUNCTIONS_H_INCLUDED
#define GLFW_INCLUDE_VULKAN
#include "camera.h"
#include "EventManager.h"
#include <string>
#include "vulkanControl.h"

Camera::Camera(VkExtent2D swapChainExtent)
{
	cameraBuffer.resize(MAX_FRAMES_IN_FLIGHT);
    cameraBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
    cameraBufferMapped.resize(MAX_FRAMES_IN_FLIGHT);
    world_to_cam_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
    proj_mat = glm::perspective(
    glm::radians(70.0f),
    swapChainExtent.width / static_cast<float>(swapChainExtent.height),
    .01f,
    2200.0f
	);

    VkDeviceSize cameraSize = sizeof(ProjectionData);
    cameraBuffer.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VulkanControl::Get()->createBuffer(cameraSize,
                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     cameraBuffer[i],
                     cameraBufferMemory[i]);
        vkMapMemory(VulkanControl::Get()->GetDevice(), cameraBufferMemory[i], 0, cameraSize, 0, &cameraBufferMapped[i]);     
	}

}

//void Camera::moveForward(float velocity) {
//	pos += velocity * view;
//}
//
//void Camera::moveHorizontal(float velocity) {
//	pos -=  velocity * right;
//}
//
//void Camera::moveVertical(float velocity) {
//	pos += velocity * up;
//}
//
//void Camera::UpdateLookAt(float xoffset, float yoffset)
//{
//	yaw += xoffset;
//	pitch += yoffset;
//	
//	// make sure that when pitch is out of bounds, screen doesn't get flipped
//	if (pitch > 89.0f)
//		pitch = 89.0f;
//	if (pitch < -89.0f)
//		pitch = -89.0f;
//
//	updateCameraVectors();
//	std::cout << "yaw: " << yaw << " pitch: " << pitch << std::endl;
//}
//
//void Camera::UpdateFov(float yoffset)
//{
//	fov -= yoffset;
//	if (fov < 1.0f)
//		fov = 1.0f;
//	if (fov > 45.0f)
//		fov = 45.0f;
//}

//void Camera::updateCameraVectors()
//{
//	// calculate the new Front vector
//	glm::vec3 front;
//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front.y = sin(glm::radians(pitch));
//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	view = glm::normalize(front);
//	
//	// also re-calculate the Right and Up vector
//	right = glm::normalize(glm::cross(view, worldUp));
//	up    = glm::normalize(glm::cross(right, view));
//}

void Camera::SetActive(uint32_t currentImage)
{
    projectionData.transform = world_to_cam_mat * proj_mat;
    glm::mat4 world_to_cam_mat_inv = world_to_cam_mat;
    glm::inverse(world_to_cam_mat_inv);
    projectionData.camPos = glm::vec3(world_to_cam_mat_inv[3][0], world_to_cam_mat_inv[3][1], world_to_cam_mat_inv[3][2]);

    projectionData.transform[1][1] *= -1;  // Flip Y axis for Vulkan coordinate system
    memcpy(cameraBufferMapped[currentImage], &projectionData, sizeof(projectionData));

}
