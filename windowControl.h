#include <GLFW/glfw3.h>

class WindowControl {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

public:
    GLFWwindow* window;
    WindowControl(void* appPointer);
    void destroyWindow(GLFWwindow* window);
    ~WindowControl();
};