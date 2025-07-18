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

    void run();

private:
   
    //std::vector<Vertex> vertices1;
    //std::vector<uint32_t> indices1;

    //std::vector<Vertex> vertices2;
    //std::vector<uint32_t> indices2;

    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    GameObject* terrain;
    GameObject* sky;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void drawFrame();

    void processInput();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    //void loadModel(std::string modelPath, std::vector<Vertex>& destVer, std::vector<uint32_t>& destIndices);

    Camera* camera;

    bool enableMouseCallback = true;
};