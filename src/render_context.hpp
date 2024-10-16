//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:34:40
//

#pragma once

#include <memory>

#include "window.hpp"

class RenderContext
{
public:
    RenderContext(std::shared_ptr<Window> window);
    ~RenderContext();

    void TestUpdate();
private:
    wgpu::Instance mInstance = nullptr;
    wgpu::Surface mSurface = nullptr;
    wgpu::Adapter mAdapter = nullptr;
    wgpu::Device mDevice = nullptr;
    wgpu::Queue mQueue = nullptr;

    wgpu::RenderPipeline mPipeline = nullptr;
};
