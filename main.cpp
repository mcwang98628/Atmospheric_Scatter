#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <chrono>
#include <array>

#include "Atmosphere.h"
#include "vulkanControl.h"
#include "Game.h"
#include <WinUser.h>

#define WIDTH 2560
#define HEIGHT 1440

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
#define MOUSE_SENSITIVITY 0.1f

Game game;

// Camera
bool firstMouse = true;
bool enableMouseCallback = true;
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;

// Sun
Sun sun = {};
void UpdateSun(float delta);

// Atmosphere
Atmosphere atmosphere = {};

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // Mouse callback implementation can be added here
}

void UpdateSun(float delta)
{
    sun.sunAngle = glm::mod(sun.sunAngle + 0.5 * delta, 3.1415926);
    sun.sunDir.y = glm::sin(sun.sunAngle);
    sun.sunDir.z = -glm::cos(sun.sunAngle);
    std::cout << "Sun angle: " << sun.sunAngle << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Scroll callback implementation can be added here
}

bool InitInstance(GLFWwindow* hwd)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    hwd = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    glfwSetCursorPosCallback(hwd, mouse_callback);
    glfwSetScrollCallback(hwd, scroll_callback);

    glfwSetInputMode(hwd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    game.Init(hwd, WIDTH, HEIGHT);
    return true;
}

int main() {
    GLFWwindow* hwd = nullptr;
    MSG msg;
    if(!InitInstance(hwd))
    {
        return EXIT_FAILURE;
    }
    
    bool running = true;
    while (running)
    {
        // Check to see if any messages are waiting in the queue
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
            case WM_KEYDOWN:
            {
                uint32_t param = (uint32_t)msg.wParam;
                if (VK_ESCAPE == param)
                    running = false;
                break;
            }
            case WM_KEYUP:
            {
                uint32_t param = (uint32_t)msg.wParam;
                // Handle key up events
            }
            break;
            case WM_QUIT:
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (running)
        {
            // Process game events
            game.ProcessEvents();
            
            // Update game logic
            game.Update(0.016f);
            
            // Render frame
            game.RenderFrame();
        }
    }

    game.ShutDown();

    return EXIT_SUCCESS;
}