#pragma once

#include "stdafx.h"
#include "vulkanControl.h"
#include "engineMath.h"

class Camera {
public:
    Camera();
    ~Camera();

    float fov = 45.f;
    float near = 0.01;
    float far = 2200.f;
    float yaw = 0.0f;
    float pitch = 0.0f;


    struct CameraBuffer {
        Matrix4 viewProjection;
        Vector3 cameraPosition;
        float padding;
    };

    void Init();
    void moveForward(float velocity);
    void moveHorizontal(float velocity);
    void moveVertical(float velocity);
    void UpdateCameraTransform(float xoffset, float yoffset);
    
    void updateCameraBuffer(uint32_t currentImage);

    void ProcessInput(double xoffset, double yoffset);

    void SetCamData(Vector3 position, Quaternion Rotation);

    void PrintCurrentCamMatrix();

    // Descriptor management methods

private:
    std::vector<VkBuffer>        cameraBuffer;
    std::vector<VkDeviceMemory>  cameraBufferMemory;
    std::vector<void*>           cameraBufferMapped;
    
    void updateDescriptorSets();

    Matrix4 projectionMatrix;
    Matrix4 worldToCamMatrix;

    CameraBuffer cameraData;

    // Camera
    bool firstMouse;
    float lastX;
    float lastY;

    bool isPrinted = false;

};
