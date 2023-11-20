#include <GLFW/glfw3.h>

class WindowInit {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

public:
    GLFWwindow* window;
    WindowInit(void* appPointer);
    GLFWwindow* initWindow(void* appPointer);
    void destroyWindow(GLFWwindow* window);
    ~WindowInit();
};