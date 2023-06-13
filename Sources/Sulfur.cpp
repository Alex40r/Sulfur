#include "Utils/Utils.hpp"

#include "Graphics/Devices/Instance.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"

#include "Graphics/GraphicsContext.hpp"

#include "Graphics/Swapchains/Swapchain.hpp"
#include "Graphics/Swapchains/SwapchainTexture.hpp"
#include "Graphics/Swapchains/WindowSurface.hpp"

#include "Graphics/Windows/Window.hpp"
#include "Graphics/Windows/WindowContext.hpp"

#include "Graphics/RenderPasses/Framebuffer.hpp"
#include "Graphics/RenderPasses/RenderAttachment.hpp"
#include "Graphics/RenderPasses/RenderDependency.hpp"
#include "Graphics/RenderPasses/RenderPass.hpp"
#include "Graphics/RenderPasses/RenderSubpass.hpp"

#include "Graphics/CommandBuffers/CommandBuffer.hpp"
#include "Graphics/CommandBuffers/CommandPool.hpp"

#include "Graphics/Synchronizations/Fence.hpp"
#include "Graphics/Synchronizations/Semaphore.hpp"

#include "Graphics/Descriptors/DescriptorLayout.hpp"
#include "Graphics/Descriptors/DescriptorPool.hpp"
#include "Graphics/Descriptors/DescriptorSet.hpp"

#include "Graphics/Resources/Textures/ITexture.hpp"
#include "Graphics/Resources/Textures/TextureView.hpp"

#include "Graphics/Pipelines/GraphicsPipeline.hpp"
#include "Graphics/Pipelines/PipelineLayout.hpp"
#include "Graphics/Pipelines/Shader.hpp"

#include "Graphics/Memory/Memory.hpp"
#include "Graphics/Memory/MemoryHeap.hpp"
#include "Graphics/Memory/MemoryType.hpp"

#include "Graphics/Resources/Buffers/Buffer.hpp"

#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* ---- ---- ---- ---- */

Handle<GraphicsContext> context;
Handle<WindowContext> window_context;
Handle<Window> window;

Handle<Instance> instance;
Handle<PhysicalDevice> physical_device;

Handle<LogicalDevice> logical_device;

Handle<CommandQueue> graphics_queue;

Handle<RenderAttachment> color_attachment;
Handle<RenderSubpass> subpass;
Handle<RenderDependency> dependency;
Handle<RenderPass> render_pass;

Handle<CommandPool> command_pool;
Handle<CommandBuffer> command_buffer;

Handle<DescriptorPool> descriptor_pool;
Handle<DescriptorLayout> descriptor_layout;
Handle<DescriptorSet> descriptor_set;

Handle<Semaphore> frame_finished_semaphore;
Handle<Semaphore> frame_available_semaphore;
Handle<Fence> fence;

Handle<WindowSurface> window_surface;
Handle<Swapchain> swapchain;

List<Handle<TextureView>> texture_views;
List<Handle<Framebuffer>> framebuffers;

Handle<PipelineLayout> pipeline_layout;
Handle<GraphicsPipeline> graphics_pipeline;

Handle<Shader> vertex_shader;

Handle<Buffer> uniform_buffer;
Handle<Buffer> stats_buffer;
Handle<Memory> memory;

struct UBO {
	UBO() = default;

	int frame;
	int width;
	int height;
	float fov;

	glm::mat4 view;
	glm::mat4 view_inverted;
	glm::mat4 projection;

	int voxelDepth;
};

UBO ubo;

#define STATS 4096

float stats[STATS];

/* ---- ---- ---- ---- */

Handle<Buffer> voxel_buffer;
Handle<Memory> voxel_memory;

#define VOXEL_CHUNK_DEPTH 4
#define VOXEL_CHUNK_SIZE (1 << (VOXEL_CHUNK_DEPTH))
#define VOXEL_CHUNK_WIDTH (VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE)
#define VOXEL_REFERENCE_BIT 0x80000000
#define VOXEL_ROOT 0

struct Chunk {
	Chunk() = default;

	uint32 bitmap[VOXEL_CHUNK_WIDTH / 32];
	uint32 data[VOXEL_CHUNK_WIDTH];

	void populateBitmap() {
		for (uint32 i = 0; i < VOXEL_CHUNK_WIDTH / 32; i++)
			bitmap[i] = 0;

		for (uint32 i = 0; i < VOXEL_CHUNK_WIDTH; i++)
			if (data[i] != 0)
				bitmap[i / 32] |= 1 << (i % 32);
	}
};

Chunk* chunkMem;
uint32 chunk_count = 1;

uint32 getMapSize() {
	return 1 << (VOXEL_CHUNK_DEPTH * ubo.voxelDepth);
}

uint32 getInChunkIndex(uint32 x, uint32 y, uint32 z) {
	return z * VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE + y * VOXEL_CHUNK_SIZE + x;
}

void write(uint32 x, uint32 y, uint32 z, uint32 value) {
	uint32 map_size = getMapSize();

	uint32 rx = 0;
	uint32 ry = 0;
	uint32 rz = 0;

	uint32 ref = VOXEL_ROOT;

	for (uint32 d = ubo.voxelDepth - 1; true; d--) {
		uint32 s = (1 << (VOXEL_CHUNK_DEPTH * d));

		uint32 dx = (x - rx) / s;
		uint32 dy = (y - ry) / s;
		uint32 dz = (z - rz) / s;

		rx += dx * s;
		ry += dy * s;
		rz += dz * s;

		uint32 in_chunk_pos = getInChunkIndex(dx, dy, dz);
		uint32& cell_value = chunkMem[ref].data[in_chunk_pos];
		sizeof(Chunk);
		if (d == 0) {
			cell_value = value;
			break;
		} else {
			if (cell_value & VOXEL_REFERENCE_BIT)
				ref = cell_value & ~VOXEL_REFERENCE_BIT;
			else {
				ref = chunk_count;
				chunk_count++;
				cell_value = ref | VOXEL_REFERENCE_BIT;
			}
		}
	}
}

uint32 read(uint32 x, uint32 y, uint32 z) {
	uint32 map_size = getMapSize();

	uint32 rx = 0;
	uint32 ry = 0;
	uint32 rz = 0;

	uint32 ref = VOXEL_ROOT;

	for (uint32 d = ubo.voxelDepth - 1; true; d--) {
		uint32 s = (1 << (VOXEL_CHUNK_DEPTH * d));

		uint32 dx = (x - rx) / s;
		uint32 dy = (y - ry) / s;
		uint32 dz = (z - rz) / s;

		rx += dx * s;
		ry += dy * s;
		rz += dz * s;

		uint32 in_chunk_pos = getInChunkIndex(dx, dy, dz);
		uint32& cell_value = chunkMem[ref].data[in_chunk_pos];

		if (d == 0) {
			return cell_value;
			break;
		} else {
			if (cell_value & VOXEL_REFERENCE_BIT)
				ref = cell_value & ~VOXEL_REFERENCE_BIT;
			else {
				return INVALID_ID;
			}
		}
	}
}

void GenerateVoxels() {
	voxel_buffer = Buffer::Create(logical_device, 100000000, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	voxel_memory = Memory::Create(logical_device, {voxel_buffer}, PREFER_LARGEST, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	ubo.voxelDepth = 2;

	chunkMem = (Chunk*)voxel_memory->MapMemory(voxel_buffer->GetSize(), 0);

	uint64 cubes = 0;

	std::cout << std::dec;

	uint32 w = 16 * 8;

	for (uint32 z = 0; z < w; z++)
		for (uint32 y = 0; y < w; y++)
			for (uint32 x = 0; x < w; x++)
				if ((x - w / 2) * (x - w / 2) + (y - w / 2) * (y - w / 2) + (z - w / 2) * (z - w / 2) <=
					(w / 2 - 16) * (w / 2 - 16)) {
					write(x, y, z, 32 + x % 32 + y % 32 + z % 32);
				}

	for (uint32 i = 0; i < chunk_count; i++)
		chunkMem[i].populateBitmap();

	chunkMem = nullptr;
	voxel_memory->UnmapMemory();
}

/* ---- ---- ---- ---- */

double x, y;

void CurCallback(GLFWwindow* window, double cx, double cy) {
	float dx = -(float)(cx - x);
	float dy = -(float)(cy - y);

	x = cx;
	y = cy;

	ubo.view = glm::rotate(glm::mat4(1.0f), glm::radians(dx * 0.1F), glm::vec3(0.0f, 1.0f, 0.0f)) * ubo.view;
	ubo.view = glm::rotate(glm::mat4(1.0f), glm::radians(dy * 0.1F), glm::vec3(1.0f, 0.0f, 0.0f)) * ubo.view;
}

float speed = 0.2f;

void ProcessMove(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ubo.view = glm::translate(ubo.view, (glm::vec3)glm::normalize(glm::inverse(ubo.view)[0]) * speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ubo.view = glm::translate(ubo.view, -(glm::vec3)glm::normalize(glm::inverse(ubo.view)[0]) * speed);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		ubo.view = glm::translate(ubo.view, (glm::vec3)glm::normalize(glm::inverse(ubo.view)[1]) * speed);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		ubo.view = glm::translate(ubo.view, -(glm::vec3)glm::normalize(glm::inverse(ubo.view)[1]) * speed);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ubo.view = glm::translate(ubo.view, (glm::vec3)glm::normalize(glm::inverse(ubo.view)[2]) * speed);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ubo.view = glm::translate(ubo.view, -(glm::vec3)glm::normalize(glm::inverse(ubo.view)[2]) * speed);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		ubo.view = glm::rotate(glm::mat4(1.0f), glm::radians(1.0F), glm::vec3(0.0f, 0.0F, 1.0F)) * ubo.view;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		ubo.view = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0F), glm::vec3(0.0f, 0.0f, 1.0f)) * ubo.view;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		speed *= 0.9F;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		speed /= 0.9F;
	}
}

void recreateSwapchain() {
	swapchain.DestroyObject();

	WindowSurface::Capabilities capabilities(window_surface, physical_device);

	VkPresentModeKHR mode;

	mode = VK_PRESENT_MODE_MAILBOX_KHR;
	mode = VK_PRESENT_MODE_FIFO_KHR;

	if (!capabilities.IsPresentModeSupported(mode))
		mode = VK_PRESENT_MODE_FIFO_KHR;

	swapchain = Swapchain::Create(
		logical_device, window_surface, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		mode, swapchain);

	const List<Handle<SwapchainTexture>>& swapchain_textures = swapchain->GetSwapchainTextures();

	Handle<SwapchainTexture> swapchain_texture = swapchain_textures[0];

	texture_views.Resize(swapchain_textures.Length());
	for (uint32 i = 0; i < swapchain_textures.Length(); i++)
		texture_views[i] = TextureView::Create(logical_device, swapchain_textures[i], VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	framebuffers.Resize(swapchain_textures.Length());
	for (uint32 i = 0; i < swapchain_textures.Length(); i++)
		framebuffers[i] = Framebuffer::Create(render_pass, {texture_views[i]}, swapchain->GetWidth(), swapchain->GetHeight());
}

void recreatePipeline() {
	graphics_pipeline.DestroyObject();

	graphics_pipeline = GraphicsPipeline::Create(
		logical_device,
		pipeline_layout,
		render_pass,
		subpass,
		{
			Shader::Stage(vertex_shader, VK_SHADER_STAGE_VERTEX_BIT, "Vertex"),
			Shader::Stage(vertex_shader, VK_SHADER_STAGE_FRAGMENT_BIT, "Pixel"),
		});
}

void recreateShaders() {
	vertex_shader.DestroyObject();

	List<uint8> shader_code;
	Utils::ReadFile("Shaders/Shaders.spv", shader_code);

	vertex_shader = Shader::Create(logical_device, shader_code);
}

void addStat(float value) {
	for (uint32 i = STATS - 1; i > 0; i--)
		stats[i] = stats[i - 1];

	stats[0] = value;

	float* ptr = (float*)memory->MapMemory(stats_buffer->GetSize(), stats_buffer->GetBindOffset());

	for (uint32 i = 0; i < STATS; i++)
		ptr[i] = stats[i];

	memory->UnmapMemory();
}

void exec() {
	context = GraphicsContext::Create();

	window_context = WindowContext::Create(context);

	glfwWindowHint(GLFW_MAXIMIZED, true); // TODO: add different constructor for window
	window = Window::Create(window_context, "Sulfur", 1920, 1080);

	instance = Instance::Create(context, window->GetRequiredInstanceExtensions(), "Sulfur");
	physical_device = instance->GetPhysicalDevice(0);

	logical_device = LogicalDevice::Create(
		physical_device, {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
		{CommandQueue::CreationInfo{physical_device->GetCommandQueueFamily(0), 1.f}});

	graphics_queue = logical_device->GetCommandQueue(0);

	color_attachment = RenderAttachment::Create(
		logical_device,
		VK_FORMAT_B8G8R8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		ATTACHMENT_CLEAR, ATTACHMENT_STORE);

	subpass = RenderSubpass::Create(
		logical_device,
		{},
		{RenderAttachment::Descriptor(color_attachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)},
		{},
		{},
		{});

	dependency = RenderDependency::Create(
		logical_device, nullptr, subpass,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);

	render_pass = RenderPass::Create(
		logical_device,
		{color_attachment},
		{subpass},
		{dependency});

	command_pool = CommandPool::Create(logical_device, graphics_queue->GetCommandQueueFamily());
	command_buffer = CommandBuffer::Create(command_pool);

	fence = Fence::Create(logical_device);
	frame_available_semaphore = Semaphore::Create(logical_device);
	frame_finished_semaphore = Semaphore::Create(logical_device);

	descriptor_pool = DescriptorPool::Create(
		logical_device,
		{DescriptorPool::Size{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1},
		 DescriptorPool::Size{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2}},
		1);

	descriptor_layout = DescriptorLayout::Create(
		logical_device,
		{DescriptorLayout::Binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT),
		 DescriptorLayout::Binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, 1, VK_SHADER_STAGE_FRAGMENT_BIT),
		 DescriptorLayout::Binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2, 1, VK_SHADER_STAGE_FRAGMENT_BIT)});

	descriptor_set = DescriptorSet::Create(descriptor_pool, descriptor_layout);

	window_surface = WindowSurface::Create(instance, window);

	pipeline_layout = PipelineLayout::Create(logical_device, {descriptor_layout});

	recreateShaders();
	recreatePipeline();
	recreateSwapchain();

	uniform_buffer = Buffer::Create(logical_device, sizeof(ubo), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	stats_buffer = Buffer::Create(logical_device, STATS * 4, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

	memory = Memory::Create(
		logical_device, {uniform_buffer, stats_buffer}, PREFER_DEVICE_LOCAL, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	GenerateVoxels();

	descriptor_set->UpdateDescriptorBuffer(0, 0, uniform_buffer);
	descriptor_set->UpdateDescriptorBuffer(1, 0, voxel_buffer);
	descriptor_set->UpdateDescriptorBuffer(2, 0, stats_buffer);

	std::cout
		<< std::endl
		<< std::endl;

	int frame = 0;

	ubo.fov = 45.0f;
	ubo.view = glm::mat4(1.0f);
	ubo.projection = glm::perspective(glm::radians(ubo.fov), swapchain->GetWidth() / (float)swapchain->GetHeight(), 0.1f, 1000.0F);

	glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window->GetGLFWWindow(), &CurCallback);

	bool full = false;
	bool lockf12 = false;
	bool lock = false;

	while (!window->ShouldClose()) {
		window->PollEvents();

		uint32 frame_id;

		ProcessMove(window->GetGLFWWindow());

		if (frame % 100 == 0) {
			recreateShaders();
			recreatePipeline();
		}

		if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_F9) == GLFW_PRESS) {
			if (!lockf12) {
				if (full)
					window->SetWindowed(1920, 1080, 200, 200);
				else
					window->SetFullscreen(window_context->GetMonitor(0));
				full = !full;
			}

			lockf12 = true;
		} else
			lockf12 = false;

		frame++;

		ubo.frame = frame;
		ubo.width = swapchain->GetWidth();
		ubo.height = swapchain->GetHeight();
		ubo.view_inverted = glm::inverse(ubo.view);

		UBO* ptr = (UBO*)memory->MapMemory(sizeof(ubo), 0);

		*ptr = ubo;

		memory->UnmapMemory();

		bool up_to_date = swapchain->AquireNextFrame(frame_id, frame_available_semaphore);

		if (!up_to_date || frame_id == INVALID_ID) {
			recreateSwapchain();
			continue;
		}

		command_pool->Reset();
		command_buffer->BeginRecording();

		command_buffer->BeginRenderPass(
			render_pass, framebuffers[frame_id], swapchain->GetWidth(), swapchain->GetHeight(),
			{{}});

		vkCmdBindPipeline(command_buffer->GetVKCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline->GetVKPipeline());

		VkDescriptorSet vk_desc_set = descriptor_set->GetVKDescriptorSet();
		vkCmdBindDescriptorSets(command_buffer->GetVKCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout->GetVKPipelineLayout(), 0, 1, &vk_desc_set, 0, nullptr);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swapchain->GetWidth();
		viewport.height = (float)swapchain->GetHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffer->GetVKCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent.width = swapchain->GetWidth();
		scissor.extent.height = swapchain->GetHeight();
		vkCmdSetScissor(command_buffer->GetVKCommandBuffer(), 0, 1, &scissor);

		vkCmdDraw(command_buffer->GetVKCommandBuffer(), 4, 1, 0, 0);

		command_buffer->EndRenderPass();

		command_buffer->FinishRecording();

		auto _stopwatch_start = std::chrono::high_resolution_clock::now();

		graphics_queue->Submit({command_buffer}, {frame_available_semaphore}, {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}, {frame_finished_semaphore}, fence);

		fence->Wait();
		fence->Reset();

		auto _stopwatch_end = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::nanoseconds>(_stopwatch_end - _stopwatch_start).count() / 1000000.0F;

		addStat(time);

		swapchain->Present(graphics_queue, frame_id, {frame_finished_semaphore});
	}

	window.DestroyObject();

	context.DestroyObject();
}

int main() {
	try {
		exec();
	} catch (std::runtime_error& e) {
		std::cout << "[\033[38;2;255;0;0m"
				  << "ERROR"
				  << "\033[m] "
				  << e.what()
				  << "\033[m"
				  << std::endl;
	}

	return 0;
}
