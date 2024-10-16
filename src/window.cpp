//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:46:29
//

#include "window.hpp"

#include <emscripten.h>
#include <cassert>

Window::Window(int width, int height)
{
    if (!glfwInit()) {
        // TODO: logger
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    mWindow = glfwCreateWindow(width, height, "MOROI", nullptr, nullptr);
}

Window::~Window()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

bool Window::IsOpen()
{
    return !glfwWindowShouldClose(mWindow);
}

void Window::Update()
{
    glfwPollEvents();
}

void Window::GetSize(int& width, int& height)
{
    glfwGetWindowSize(mWindow, &width, &height);
}

wgpu::Surface Window::CreateWGPUSurface(wgpu::Instance instance)
{
    WGPUSurface surface = reinterpret_cast<WGPUSurface>(EM_ASM_PTR({
        var context = canvas.getContext('webgpu');
        return WebGPU.mgrSurface.create(context);
    }));

    assert(surface);

    return wgpu::Surface(surface);
}
