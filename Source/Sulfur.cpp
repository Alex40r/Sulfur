#include "Utils/Utils.hpp"

#include "Graphics/GraphicsContext.hpp"

#include "Graphics/Windows/Monitor.hpp"
#include "Graphics/Windows/Window.hpp"
#include "Graphics/Windows/WindowContext.hpp"

#include "Graphics/Devices/Instance.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"

#include "Graphics/RenderPasses/RenderAttachment.hpp"
#include "Graphics/RenderPasses/RenderDependency.hpp"
#include "Graphics/RenderPasses/RenderPass.hpp"
#include "Graphics/RenderPasses/RenderSubpass.hpp"

#include "Graphics/CommandBuffers/CommandBuffer.hpp"
#include "Graphics/CommandBuffers/CommandPool.hpp"

#include <chrono>

int main() {
	Handle<GraphicsContext> context = GraphicsContext::Create();
	Handle<WindowContext> window_context = WindowContext::Create(context);
	Handle<Window> window = Window::Create(window_context, "Sulfur", 800, 600);
	Handle<Instance> instance = Instance::Create(context, {}, "Sulfur", "Sulfur");
	Handle<PhysicalDevice> physical_device = instance->GetPhysicalDevice(0);
	Handle<LogicalDevice> logical_device = LogicalDevice::Create(
		physical_device, {},
		{CommandQueueDescriptor(physical_device->GetCommandQueueFamily(0), 1.0F)});

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
		{});

	Handle<CommandPool> command_buffer_pool = CommandPool::Create(logical_device, physical_device->GetCommandQueueFamily(0));

	Handle<CommandBuffer> command_buffer = CommandBuffer::Create(command_buffer_pool);

	std::cout << "\n\n";

	while (!window->ShouldClose())
		Window::PollEvents();

	context.DestroyObject();
	return 0;
}