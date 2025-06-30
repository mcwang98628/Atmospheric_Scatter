#pragma once
#include "camera.h"
#include "vulkanControl.h"
#include "GameObject.h"
#include "Mesh.h"
#include <vector>
#include <memory>

class Game{
    public:
        Game();
        ~Game();
        void Init(GLFWwindow* hwd, int width, int height);
        void ProcessEvents();
        void Update(float deltaTime);
        void RenderFrame();
        void ShutDown();
    private:
        VulkanControl* m_vulkanController;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
        
        // Meshes
        std::unique_ptr<Mesh> m_planeMesh;
        std::unique_ptr<Mesh> m_skyMesh;
        
        // Game objects
        GameObject* m_planeObject;
        GameObject* m_skyObject;

        const std::string TERRAIN_PATH = "models/Plane.obj";
        const std::string SKY_PATH = "models/sphere.obj";
        
        uint32_t currentFrame = 0;
        bool framebufferResized = false;
        GLFWwindow* window = nullptr;
};