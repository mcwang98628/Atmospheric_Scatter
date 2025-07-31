#pragma once

#include "stdafx.h"
#include "camera.h"
#include <string>
#include <vector>
#include "GameObject.h"



class Game {
public:
    Game();
    ~Game();

    static Camera* camera;


    bool Init();
    void Update();
    void ProcessInput();
    static void ProcessMouseInput(GLFWwindow* win, double xposIn, double yposIn);
    void DrawFrame();
    void ShutDown();

private:
    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    GameObject* terrain;
    GameObject* sky;

    void initWindow();
    void initVulkan();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }


    //void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    bool enableMouseCallback = true;
};