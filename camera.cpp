#define FUNCTIONS_H_INCLUDED
#include "camera.h"
#include "EventManager.h"
#include <string>
#include <iostream>

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
