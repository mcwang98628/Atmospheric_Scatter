#include "Game.h"
#include "Engine/EngineCore.h"

//#include "Engine/camera.h"
//#include "Engine/GameObject.h"
//#include "Engine/Atmosphere.h"
#include "Engine/Input/InputManager.h"


// settings
const std::string TERRAIN_PATH = "models/Plane.obj";
const std::string SKY_PATH = "models/sphere.obj";


Game::Game()
{
}

Game::~Game()
{
}

bool Game::Init()
{
    //initWindow();
    //initVulkan();
    //InputManager::Instance()->Init();
    if(!EngineCore::Initialize(this))
    {
        return false;
    }
    
    return true;
}

void Game::initWindow()
{
    // WinApplication::Init(this);
    //glfwSetScrollCallback(WindowControl::GetWindow(), scroll_callback);

}

void Game::initVulkan()
{
    //VulkanControl* vulkanController = VulkanControl::Get();

    //vulkanController->createInstance();
    //vulkanController->setupDebugMessenger();
    //vulkanController->createSurface(WinApplication::GetWindow());
    //vulkanController->pickPhysicalDevice();
    //vulkanController->createLogicalDevice();
    //vulkanController->createSwapChain();
    //vulkanController->createImageViews();
    //vulkanController->createRenderPass();
    //vulkanController->createDescriptorSetLayout();
    //vulkanController->createPipelineLayout();
    ////vulkanController->createGraphicsPipeline("shaders/terrainVert.spv", "shaders/terrainFrag.spv", vulkanController->graphicsPipeline1);
    ////vulkanController->createGraphicsPipeline("shaders/skyVert.spv", "shaders/skyFrag.spv", vulkanController->graphicsPipeline2);
    //vulkanController->createCommandPool();
    //vulkanController->createDepthResources();
    //vulkanController->createFramebuffers();
    //vulkanController->createTextureImage(TEXTURE_PATH);
    //vulkanController->createTextureImageView();
    //vulkanController->createTextureSampler();

    //vulkanController->createDescriptorPool();
    //vulkanController->createDescriptorSets();

    //terrain = new GameObject();
    //sky = new Atmosphere();
    //terrain->LoadModel(TERRAIN_PATH);
    //sky->LoadModel(SKY_PATH);
    //terrain->BindGraphicPipeline("shaders/terrainVert.spv", "shaders/terrainFrag.spv");
    //sky->BindGraphicPipeline("shaders/skyVert.spv", "shaders/skyFrag.spv");

    //// vulkanController->CreateSun(&sun);
    //terrain->CreateVertexBuffer();
    //terrain->CreateIndexBuffer();

    //sky->CreateVertexBuffer();
    //sky->CreateIndexBuffer();

    //// Initialize camera descriptor sets after VulkanControl creates the main descriptor sets
    //camera->Init();
    
    /*vulkanController->createCommandBuffers();
    vulkanController->createSyncObjects();*/
}

void Game::InitUserInput()
{

}

void Game::Update(float deltaTime)
{
    //const auto currTime = static_cast<float>(glfwGetTime());
    //deltaTime = currTime - lastFrame;
    //lastFrame = currTime;

    //camera->updateCameraBuffer(VulkanControl::Get()->GetCurrentFrameIndex());
    EngineCore::Update();
    //drawFrame();
    //processInput();

    //UpdateSun(deltaTime);
    // VulkanControl::Get()->WaitIdle();
}

void Game::ShutDown()
{
    // VulkanControl::Get()->WaitIdle();
    EngineCore::EndFrame();
    delete camera;
    // VulkanControl* vulkanController = VulkanControl::Get();
    // WinApplication* wd = WinApplication::Get();
    delete terrain;
    delete sky;
    // vulkanController->cleanUp();
    // wd->destroyWindow(WinApplication::GetWindow());
    EngineCore::Shutdown();
}

void Game::DrawFrame()
{
    // VulkanControl* vulkanController = VulkanControl::Get();
    // vulkanController->BeginRenderer();
    EngineCore::BeginFrame();
    
    // sky->Draw(vulkanController->GetCommandBuffer(), vulkanController->GetCurrentFrameIndex());
    // terrain->Draw(vulkanController->GetCommandBuffer(), vulkanController->GetCurrentFrameIndex());
    // vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, vulkanController->graphicsPipeline2, vulkanController->vertexBuffer2, vulkanController->indexBuffer2, indices2);  // Draw blue sky first (background)
    // vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, vulkanController->graphicsPipeline1, vulkanController->vertexBuffer1, vulkanController->indexBuffer1, indices1);  // Draw green ground second (foreground)
    EngineCore::EndFrame();

    // vulkanController->EndRenderer();

}

void Game::BindInput()
{
    inputHandler.BindKey(InputEvents::Key_ESCAPE, [this]() {
        if (running) { *running = false; }
    });

    float cameraSpeed = 25.f;

   /* inputHandler.BindKey(InputEvents::Key_W, [this, cameraSpeed]() {
        camera->moveForward(cameraSpeed);
    });

    inputHandler.BindKey(InputEvents::Key_S, [this, cameraSpeed]() {
        camera->moveForward(-cameraSpeed);
    });

    inputHandler.BindKey(InputEvents::Key_A, [this, cameraSpeed]() {
        camera->moveForward(-cameraSpeed);
    });
    inputHandler.BindKey(InputEvents::Key_D, [this, cameraSpeed]() {
        camera->moveForward(-cameraSpeed);
    });
    inputHandler.BindKey(InputEvents::Key_UP, [this]() {
        sky->UpdateSun(0.01f);
    });
    inputHandler.BindKey(InputEvents::Key_DOWN, [this]() {
        sky->UpdateSun(-.01f);
    });
    inputHandler.BindKey(InputEvents::Key_F, [this]() {
        camera->PrintCurrentCamMatrix();
    });
    inputHandler.BindMouseMove([this](float xposIn, float yposIn) {
        camera->ProcessInput(xposIn, yposIn);
    });
    InputManager::Instance()->RegisterHandler(&inputHandler);*/
    /*if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (enableMouseCallback)
        {ws
            glfwSetInputMode(WindowControl::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            enableMouseCallback = false;
        }
        else {
            glfwSetInputMode(WindowControl::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            enableMouseCallback = true;
        }
    }*/

    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
    //     camera->moveForward(cameraSpeed);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
    //     camera->moveForward(-cameraSpeed);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
    //     camera->moveHorizontal(-cameraSpeed);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
    //     camera->moveHorizontal(cameraSpeed);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
    //     camera->moveVertical(-cameraSpeed);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
    //     camera->moveVertical(cameraSpeed);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_UP) == GLFW_PRESS)
    //     sky->UpdateSun(0.01f);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
    //     sky->UpdateSun(-.01f);
    // if (glfwGetKey(WindowControl::GetWindow(), GLFW_KEY_F) == GLFW_PRESS)
    //     camera->PrintCurrentCamMatrix();
}

//void Game::ProcessMouseInput(GLFWwindow* win, double xposIn, double yposIn)
//{
//    if (ENABLE_MOUSE_CALLBACK)
//        camera->ProcessInput(xposIn, yposIn);
//}