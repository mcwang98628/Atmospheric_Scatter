#pragma once

#include "stdafx.h"
#include "engineMath.h"

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;

namespace StudyEngine {
    enum CameraDirection {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    class STUDY_ENGINE Camera {
    public:
        
        Camera();
        ~Camera();

        struct CameraBuffer {
            Matrix4 viewProjection;
            Vector3 cameraPosition;
            float padding;
        };

        void AddMovement(Vector3 dir);
        void RotateCamera(float xoffset, float yoffset);
    
        void UpdateCameraTransform(float dTime);

        void ProcessInput(double xoffset, double yoffset);

        void SetCamData(Vector3 position, Quaternion Rotation);
        
        void UploadCameraBuffer();
        void PrintCurrentCamMatrix();

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

        float speed = 25.f;

        bool isPrinted = false;
        bool dirty = false;

        float fov = 45.f;
        float near = 0.01;
        float far = 2200.f;
        float yaw = 0.0f;
        float pitch = 0.0f;

        Vector3 movementInput = Vector3(0.f, 0.f, 0.f);
    };
}
