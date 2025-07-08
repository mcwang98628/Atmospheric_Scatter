
#include "stdafx.h"
#include "Game.h"


//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//    
//    float xoffset = (xpos - lastX) * MOUSE_SENSITIVITY;
//    float yoffset = (lastY - ypos) * MOUSE_SENSITIVITY;
//    lastX = xpos;
//    lastY = ypos;
//    
//    if (enableMouseCallback)
//        camera.UpdateLookAt(xoffset, yoffset);
//}
//
//void UpdateSun(float delta)
//{
//    sun.sunAngle = glm::mod(sun.sunAngle + 0.5 * delta, 3.1415926);
//    sun.sunDir.y = glm::sin(sun.sunAngle);
//    sun.sunDir.z = -glm::cos(sun.sunAngle);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    // if (enableMouseCallback)
//    //     UpdateSun(static_cast<float>(yoffset));
//}


int main() {
    Game app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}