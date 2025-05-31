#include <GLFW/glfw3.h>

class WindowControl {
    const uint32_t WIDTH = 2560;
    const uint32_t HEIGHT = 1440;

public:
    GLFWwindow* window;
    WindowControl(void* appPointer);
    void destroyWindow(GLFWwindow* window);
    ~WindowControl();
};