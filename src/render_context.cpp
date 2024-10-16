//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:39:19
//

#include "render_context.hpp"

#include <emscripten.h>
#include <iostream>

wgpu::Adapter GetAdapter(wgpu::Instance instance, wgpu::RequestAdapterOptions const * options) {
	struct UserData {
		WGPUAdapter adapter = nullptr;
		bool requestEnded = false;
	};
	UserData userData;

	auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * pUserData) {
		UserData& userData = *reinterpret_cast<UserData*>(pUserData);
		if (status == WGPURequestAdapterStatus_Success) {
			userData.adapter = adapter;
		} else {
			std::cout << "Could not get WebGPU adapter: " << message << std::endl;
		}
		userData.requestEnded = true;
	};

    instance.RequestAdapter(options, onAdapterRequestEnded, (void*)&userData);

#ifdef __EMSCRIPTEN__
	while (!userData.requestEnded) {
		emscripten_sleep(100);
	}
#endif

	return userData.adapter;
}

wgpu::Device GetDevice(wgpu::Adapter adapter, wgpu::DeviceDescriptor const * descriptor) {
	struct UserData {
		WGPUDevice device = nullptr;
		bool requestEnded = false;
	};
	UserData userData;

	auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, char const * message, void * pUserData) {
		UserData& userData = *reinterpret_cast<UserData*>(pUserData);
		if (status == WGPURequestDeviceStatus_Success) {
			userData.device = device;
		} else {
			std::cout << "Could not get WebGPU device: " << message << std::endl;
		}
		userData.requestEnded = true;
	};

	adapter.RequestDevice(descriptor, onDeviceRequestEnded, (void*)&userData);

#ifdef __EMSCRIPTEN__
	while (!userData.requestEnded) {
		emscripten_sleep(100);
	}
#endif // __EMSCRIPTEN__

	return userData.device;
}

RenderContext::RenderContext(std::shared_ptr<Window> window)
{
    // Create instance
    mInstance = wgpu::CreateInstance();

    // Create surface
    mSurface = window->CreateWGPUSurface(mInstance);

    // Create adapter
    wgpu::RequestAdapterOptions options = {};
    options.compatibleSurface = mSurface;

    mAdapter = GetAdapter(mInstance, &options);

	// Create device
	wgpu::DeviceDescriptor deviceDescriptor = {};
	mDevice = GetDevice(mAdapter, &deviceDescriptor);

	// Get queue
	mQueue = mDevice.GetQueue();

	// Configure surface
	int width, height;
	window->GetSize(width, height);

	wgpu::SurfaceConfiguration surface = {};
	surface.width = width;
	surface.height = height;
	surface.usage = wgpu::TextureUsage::RenderAttachment;
	surface.format = mSurface.GetPreferredFormat(mAdapter);
	surface.viewFormatCount = 0;
	surface.viewFormats = nullptr;
	surface.device = mDevice;
	surface.presentMode = wgpu::PresentMode::Fifo;
	surface.alphaMode = wgpu::CompositeAlphaMode::Auto;

	mSurface.Configure(&surface);
}

RenderContext::~RenderContext()
{

}

void RenderContext::TestUpdate()
{
	wgpu::SurfaceTexture surfaceTexture;
  	mSurface.GetCurrentTexture(&surfaceTexture);

  	wgpu::RenderPassColorAttachment attachment{
  	    .view = surfaceTexture.texture.CreateView(),
  	    .loadOp = wgpu::LoadOp::Clear,
  	    .storeOp = wgpu::StoreOp::Store,
		.clearValue = wgpu::Color{193.0f / 255.0f, 225.0f / 255.0f, 193.0f / 255.0f, 1.0f}};

  	wgpu::RenderPassDescriptor renderpass{.colorAttachmentCount = 1,
  	                                      .colorAttachments = &attachment};

  	wgpu::CommandEncoder encoder = mDevice.CreateCommandEncoder();
  	wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
  	pass.End();
  	wgpu::CommandBuffer commands = encoder.Finish();
  	mQueue.Submit(1, &commands);
}
