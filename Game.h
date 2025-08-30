#pragma once

#include <string>
#include <vector>
#include "camera.h"
#include "GameObject.h"
#include "Atmosphere.h"
#include "ApplicationInputHandler.h"


class Game {
public:
    Game();
    ~Game();

    Camera* camera;


    bool Init();
    void Update(float deltaTime);
    void BindInput();
    void DrawFrame();
    void ShutDown();
    void SetRunning(bool &runningFlag) {running = &runningFlag;}

private:
    bool* running;

    GameObject* terrain;
    Atmosphere* sky;

    ApplicationInputHandler inputHandler;
    /*bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }*/


    //void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    bool enableMouseCallback = true;
};