#pragma once

#include "stdafx.h"
#include "vulkanControl.h"
#include "windowControl.h"

class Camera {
public:
    Camera();
    ~Camera();
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 view;
    glm::vec3 right;
    glm::vec3 proj;

    glm::vec3 worldUp;
    
    float yaw;
    float pitch;
    float fov;
    float speed;
    float near;
    float far;

    void moveForward(float velocity);
    void moveHorizontal(float velocity);
    void moveVertical(float velocity);
    
    void UpdateLookAt(float xoffset, float yoffset);
    void UpdateFov(float yoffset);
    void createCamera();

    void updateCameraBuffer(uint32_t currentImage);

    // Descriptor management methods

    VkDescriptorSetLayoutBinding getCameraDescriptorLayoutBinding();

private:
    std::vector<VkBuffer>        cameraBuffer;
    std::vector<VkDeviceMemory>  cameraBufferMemory;
    std::vector<void*>           cameraBufferMapped;
    
    // Descriptor set management
    std::vector<VkWriteDescriptorSet> descriptorWrites;
    

    //static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    void updateCameraVectors();

    void updateDescriptorSets();


    CameraBuffer cameraData;

    // Camera
    static bool firstMouse;
    static float lastX;
    static float lastY;
};
