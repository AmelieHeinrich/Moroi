//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:13:06
//

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

class App
{
public:
    App();
    ~App();

    void OnFrame();
private:
    GLFWwindow* mWindow;
};
