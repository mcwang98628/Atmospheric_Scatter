#include "Game.h"

#include "Input/InputManager.h"
#include "Application/WinApplication.h"
#include "Render/Renderer.h"

using namespace StudyEngine;
// settings
const std::string TERRAIN_PATH = "models/Plane.obj";
const std::string SKY_PATH = "models/sphere.obj";


Game::Game()
{
    running = new bool(false);
}

Game::~Game()
{
}

bool Game::Init()
{
    WinApplication::Init(this);
    Renderer::Init();
    InputManager::Init();
    
    terrain = new GameObject();
    sky = new Atmosphere();
    terrain->LoadModel(TERRAIN_PATH);
    sky->LoadModel(SKY_PATH);
    terrain->CreateVertexBuffer();
    terrain->CreateIndexBuffer();

    sky->CreateVertexBuffer();
    sky->CreateIndexBuffer();
    terrain->BindGraphicPipeline("shaders/terrainVert.spv", "shaders/terrainFrag.spv");
    sky->BindGraphicPipeline("shaders/skyVert.spv", "shaders/skyFrag.spv");
    camera = new Camera();
    BindInput();
    return true;
}

//void Game::initWindow()
//{
//    // WinApplication::Init(this);
//    //glfwSetScrollCallback(WindowControl::GetWindow(), scroll_callback);
//
//}

//void Game::initVulkan()
//{
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
//}

//void Game::InitUserInput()
//{
//
//}

void Game::Update(float deltaTime)
{
    WinApplication::Update();
    if (InputManager::IsKeyPressed(InputEvents::Key_W))
    {
        camera->AddMovement(Vector3(0, 0, 1));
    }
    if (InputManager::IsKeyPressed(InputEvents::Key_S))
    {
        camera->AddMovement(Vector3(0, 0, -1));
    }
    if (InputManager::IsKeyPressed(InputEvents::Key_A))
    {
        camera->AddMovement(Vector3(-1, 0, 0));
    }
    if (InputManager::IsKeyPressed(InputEvents::Key_D))
    {
        camera->AddMovement(Vector3(1, 0, 0));
    }
    if (InputManager::IsKeyPressed(InputEvents::Key_UP))
    {
        sky->UpdateSun(0.01f);
    }
    if (InputManager::IsKeyPressed(InputEvents::Key_DOWN))
    {
        sky->UpdateSun(-0.01f);
    }
    camera->UpdateCameraTransform(deltaTime);
    //processInput();
    //UpdateSun(deltaTime);
    // VulkanControl::Get()->WaitIdle();
}

void Game::ShutDown()
{
    // VulkanControl::Get()->WaitIdle();
    //EngineCore::EndFrame();
    delete camera;
    // VulkanControl* vulkanController = VulkanControl::Get();
    // WinApplication* wd = WinApplication::Get();
    delete terrain;
    delete sky;
    // vulkanController->cleanUp();
    // wd->destroyWindow(WinApplication::GetWindow());
    //EngineCore::Shutdown();
}

void Game::DrawFrame()
{
    Renderer::BeginRender();
    // VulkanControl* vulkanController = VulkanControl::Get();
    // vulkanController->BeginRenderer();
    //EngineCore::BeginFrame();
    camera->UploadCameraBuffer();
    sky->Draw();
    terrain->Draw();
    // vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, vulkanController->graphicsPipeline2, vulkanController->vertexBuffer2, vulkanController->indexBuffer2, indices2);  // Draw blue sky first (background)
    // vulkanController->recordCommandBuffer(vulkanController->commandBuffers[currentFrame], imageIndex, currentFrame, vulkanController->graphicsPipeline1, vulkanController->vertexBuffer1, vulkanController->indexBuffer1, indices1);  // Draw green ground second (foreground)
    //EngineCore::EndFrame();
     Renderer::EndRender();

    // vulkanController->EndRenderer();

}

void Game::BindInput()
{
    inputHandler.BindKey(InputEvents::Key_ESCAPE, [this]() {
        if (running) { *running = false; }
    });

    //inputHandler.BindKey(InputEvents::Key_W, [this]() {
    //    camera->AddMovement(Vector3(0, 0, 1));
    //});

    //inputHandler.BindKey(InputEvents::Key_S, [this]() {
    //    camera->AddMovement(Vector3(0, 0, -1));
    //});

    //inputHandler.BindKey(InputEvents::Key_A, [this]() {
    //    camera->AddMovement(Vector3(-1, 0, 0));
    //});
    //inputHandler.BindKey(InputEvents::Key_D, [this]() {
    //    camera->AddMovement(Vector3(1, 0, 0));
    //});
    //inputHandler.BindKey(InputEvents::Key_UP, [this]() {
    //    sky->UpdateSun(0.01f);
    //});
    //inputHandler.BindKey(InputEvents::Key_DOWN, [this]() {
    //    sky->UpdateSun(-.01f);
    //});
    inputHandler.BindKey(InputEvents::Key_F, [this]() {
        camera->PrintCurrentCamMatrix();
    });
    inputHandler.BindMouseMove([this](float xposIn, float yposIn) {

        camera->ProcessInput(xposIn, yposIn);
    });
    InputManager::RegisterHandler(&inputHandler);
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
}