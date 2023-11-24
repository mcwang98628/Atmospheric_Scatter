#define FUNCTIONS_H_INCLUDED
#include "camera.h"
#include "EventManager.h"
#include <string>
#include <iostream>

static EventManager em;

void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		em.emit("vertical", .1f);
	}
	if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
		em.emit("vertical", -.1f);
	}
	if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
		em.emit("horizontal", -.1f);
	}
	if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
		em.emit("horizontal", .1f);
	}
}

void Camera::moveHorizontal(float velocity) {
	float x = pos[0];
	x += velocity;
	glm::vec3 newPos = glm::vec3(x, pos[1], pos[2]);
	pos = newPos;
}

void Camera::moveVertical(float velocity) {
	float y = pos[1];
	y += velocity;
	glm::vec3 newPos = glm::vec3(pos[0], y, pos[2]);
	pos = newPos;
}

void Camera::init(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyEvent);
	em.on("horizontal", [this](float velocity) { this->moveHorizontal(velocity); });
	em.on("vertical", [this](float velocity) { this->moveVertical(velocity); });

}