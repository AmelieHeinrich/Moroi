//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:43:50
//

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <webgpu/webgpu_cpp.h>

class Window
{
public:
    Window(int width, int height);
    ~Window();

    bool IsOpen();
    void Update();

    void GetSize(int& width, int& height);
    wgpu::Surface CreateWGPUSurface(wgpu::Instance instance);
private:
    GLFWwindow* mWindow;
};
