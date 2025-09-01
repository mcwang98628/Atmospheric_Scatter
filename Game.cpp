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
}

void Game::ShutDown()
{
    Renderer::PreShutDown();
    delete camera;
    delete terrain;
    delete sky;

    Renderer::ShutDown();
}

void Game::DrawFrame()
{
    Renderer::BeginRender();

    camera->UploadCameraBuffer();
    sky->Draw();
    terrain->Draw();
   
     Renderer::EndRender();

}

void Game::BindInput()
{
    inputHandler.BindKey(InputEvents::Key_ESCAPE, [this]() {
        if (running) { *running = false; }
    });

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