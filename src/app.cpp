//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:15:26
//

#include "app.hpp"

App::App()
{
    if (!glfwInit()) {
        // TODO
    }
    mWindow = glfwCreateWindow(1280, 720, "Emscripten Window", nullptr, nullptr);
}

App::~App()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void App::OnFrame()
{
    glfwPollEvents();
}
