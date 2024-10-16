//
// $Notice: Xander Studios @ 2024
// $Author: Amélie Heinrich
// $Create Time: 2024-10-16 22:39:19
//

#include "render_context.hpp"

#include <emscripten.h>
#include <iostream>

constexpr const char* SHADER_SOURCE = R"(
	struct VertexOutput {
    	@builtin(position) position: vec4f,
    	@location(0) color: vec3f,
	};

	@vertex
	fn vs_main(@builtin(vertex_index) i : u32) -> VertexOutput {
    	const pos = array(vec2f(0, 0.5), vec2f(-0.5, -0.5), vec2f(0.5, -0.5));
		const color = array(vec3f(1, 0, 0), vec3(0, 1, 0), vec3f(0, 0, 1));

		var out: VertexOutput;
		out.position = vec4f(pos[i], 0.0, 1.0);
		out.color = color[i];
		return out;
	}

	@fragment fn fragmentMain(in: VertexOutput) -> @location(0) vec4f {
		return vec4f(in.color, 1.0);
	}
)";

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

	// Pipeline
	wgpu::ShaderModuleWGSLDescriptor wgslDesc{};
  	wgslDesc.code = SHADER_SOURCE;

  	wgpu::ShaderModuleDescriptor shaderModuleDescriptor{
  	    .nextInChain = &wgslDesc};
  	wgpu::ShaderModule shaderModule =
  	    mDevice.CreateShaderModule(&shaderModuleDescriptor);

  	wgpu::ColorTargetState colorTargetState{.format = surface.format};

  	wgpu::FragmentState fragmentState{.module = shaderModule,
  	                                  .targetCount = 1,
  	                                  .targets = &colorTargetState};

  	wgpu::RenderPipelineDescriptor descriptor{
  	    .vertex = {.module = shaderModule},
  	    .fragment = &fragmentState};
  	mPipeline = mDevice.CreateRenderPipeline(&descriptor);
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
	pass.SetPipeline(mPipeline);
	pass.Draw(3);
  	pass.End();
  	wgpu::CommandBuffer commands = encoder.Finish();
  	mQueue.Submit(1, &commands);
}
