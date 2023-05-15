#include "Utils/Utils.hpp"

#include "Graphics/Descriptors/DescriptorLayout.hpp"
#include "Graphics/Descriptors/DescriptorPool.hpp"
#include "Graphics/Descriptors/DescriptorSet.hpp"
#include "Graphics/Devices/Instance.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/RenderPasses/RenderAttachment.hpp"
#include "Graphics/RenderPasses/RenderDependency.hpp"
#include "Graphics/RenderPasses/RenderPass.hpp"
#include "Graphics/RenderPasses/RenderSubpass.hpp"
#include "Graphics/Windows/Monitor.hpp"
#include "Graphics/Windows/Window.hpp"
#include "Graphics/Windows/WindowContext.hpp"
#include "Graphics/Windows/WindowSurface.hpp"

#include <chrono>

int main() {
	Handle<GraphicsContext> graphics_context = GraphicsContext::Create();

	Handle<WindowContext> window_context = WindowContext::Create(graphics_context);

	Handle<Monitor> monitor = window_context->GetMonitors()[0];
	Handle<Window> window = Window::Create(window_context, 1920 / 2, 1080 / 2, "Sulfur");

	Handle<Instance> instance = Instance::Create(graphics_context, window->GetRequiredInstanceExtentions(), "Sulfur", "Sulfur");

	Handle<PhysicalDevice> physical_device = instance->GetPhysicalDevices()[0];

	Handle<LogicalDevice> logical_device = LogicalDevice::Create(
		physical_device,
		{CommandQueue::CreationInfo(physical_device->GetCommandQueueFamilies()[0], 1.0F)}, {});

	Handle<RenderAttachment> color_attachment = RenderAttachment::Create(
		logical_device,
		VK_FORMAT_B8G8R8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		ATTACHMENT_CLEAR, ATTACHMENT_STORE);

	Handle<RenderAttachment> depth_attachment = RenderAttachment::Create(
		logical_device,
		VK_FORMAT_D32_SFLOAT,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		ATTACHMENT_CLEAR, ATTACHMENT_DONT_STORE);

	Handle<RenderSubpass> subpass = RenderSubpass::Create(
		logical_device,
		{},
		{RenderAttachmentDescriptor(color_attachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)},
		{},
		RenderAttachmentDescriptor(depth_attachment, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL),
		{});

	Handle<RenderDependency> dependency = RenderDependency::Create(
		logical_device, nullptr, subpass,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);

	Handle<RenderPass> render_pass = RenderPass::Create(
		logical_device,
		{color_attachment, depth_attachment},
		{subpass},
		{dependency});

	Handle<WindowSurface> surface = WindowSurface::Create(instance, window);

	Handle<DescriptorPool> pool = DescriptorPool::Create(
		logical_device,
		{DescriptorPool::Size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1)},
		1);

	Handle<DescriptorLayout> layout = DescriptorLayout::Create(logical_device, {DescriptorLayout::Binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, 1, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)});

	Handle<DescriptorSet> set = DescriptorSet::Create(logical_device, layout, pool);

	while (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_Q) != GLFW_PRESS) {
		window->PollEvents();
	}

	std::cout << "\n";

	graphics_context.DestroyObject();

	return 0;
}