//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:13:06
//

#pragma once

#include <memory>

#include "window.hpp"
#include "render_context.hpp"

class App
{
public:
    App();
    ~App();

    void OnFrame();
private:
    std::shared_ptr<Window> mWindow;
    std::shared_ptr<RenderContext> mRenderContext;
};
