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
    glm::vec3 proj;
    float speed;
    float fovY;
    float near;
    float far;

	
    void init(GLFWwindow* window);
private:
    void moveHorizontal(float velocity);
    void moveVertical(float velocity);
};
#endif