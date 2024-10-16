//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:39:19
//

#include "render_context.hpp"

#include <emscripten.h>
#include <iostream>

wgpu::Adapter GetAdapter(wgpu::Instance instance, wgpu::RequestAdapterOptions const * options) {
	// A simple structure holding the local information shared with the
	// onAdapterRequestEnded callback.
	struct UserData {
		WGPUAdapter adapter = nullptr;
		bool requestEnded = false;
	};
	UserData userData;

	// Callback called by wgpuInstanceRequestAdapter when the request returns
	// This is a C++ lambda function, but could be any function defined in the
	// global scope. It must be non-capturing (the brackets [] are empty) so
	// that it behaves like a regular C function pointer, which is what
	// wgpuInstanceRequestAdapter expects (WebGPU being a C API). The workaround
	// is to convey what we want to capture through the pUserData pointer,
	// provided as the last argument of wgpuInstanceRequestAdapter and received
	// by the callback as its last argument.
	auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * pUserData) {
		UserData& userData = *reinterpret_cast<UserData*>(pUserData);
		if (status == WGPURequestAdapterStatus_Success) {
			userData.adapter = adapter;
		} else {
			std::cout << "Could not get WebGPU adapter: " << message << std::endl;
		}
		userData.requestEnded = true;
	};

	// Call to the WebGPU request adapter procedure
    instance.RequestAdapter(options, onAdapterRequestEnded, (void*)&userData);

	// We wait until userData.requestEnded gets true
#ifdef __EMSCRIPTEN__
	while (!userData.requestEnded) {
		emscripten_sleep(100);
	}
#endif

	return userData.adapter;
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
}

RenderContext::~RenderContext()
{

}
