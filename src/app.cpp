//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:15:26
//

#include "app.hpp"

App::App()
{
    mWindow = std::make_shared<Window>(1280, 720);
    mRenderContext = std::make_shared<RenderContext>(mWindow);
}

App::~App()
{

}

void App::OnFrame()
{
    mWindow->Update();
}
