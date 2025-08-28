#pragma once

#include <string>
#include <vector>
#include "ApplicationInputHandler.h"


class Camera;
class GameObject;
class Atmosphere;


class Game {
public:
    Game();
    ~Game();

    Camera* camera;


    bool Init();
    void Update(float deltaTime);
    void BindInput();
    //static void ProcessMouseInput(GLFWwindow* win, double xposIn, double yposIn);
    void DrawFrame();
    void ShutDown();
    void SetRunning(bool runningFlag) {*running = runningFlag;}

private:
    // uint32_t currentFrame = 0;
    bool* running;


    GameObject* terrain;
    Atmosphere* sky;

    void initWindow();
    void initVulkan();
    void InitUserInput();

    ApplicationInputHandler inputHandler;
    /*bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }*/


    //void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    bool enableMouseCallback = true;
};