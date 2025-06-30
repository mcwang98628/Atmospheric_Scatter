#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "vulkanControl.h"

#ifndef CAMERA
#define CAMERA
class Camera {
public:
    Camera(VkExtent2D swapChainExtent);
    virtual ~Camera();

    struct ProjectionData {
        alignas(16) glm::mat4 transform;
        alignas(4) glm::vec3 camPos;
    };

    float speed;

    void moveForward(float velocity);
    void moveHorizontal(float velocity);
    void moveVertical(float velocity);
    
    void UpdateLookAt(float xoffset, float yoffset);
    void UpdateFov(float yoffset);

    void SetActive(uint32_t currentImage);

private:
    ProjectionData projectionData;
    glm::mat4 world_to_cam_mat;
    glm::mat4 proj_mat;
    void updateCameraVectors();

    std::vector<VkBuffer>        cameraBuffer;
    std::vector<VkDeviceMemory>  cameraBufferMemory;
    std::vector<void*>           cameraBufferMapped;
};
#endif