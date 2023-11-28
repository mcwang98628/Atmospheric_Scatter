#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <GLFW/glfw3.h>

#ifndef CAMERA
#define CAMERA
class Camera {
public:
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

private:
    void updateCameraVectors();
};
#endif