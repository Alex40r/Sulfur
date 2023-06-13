#include "CommandBuffer.hpp"

#include "CommandPool.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/RenderPasses/RenderPass.hpp"
#include "Graphics/RenderPasses/Framebuffer.hpp"

CommandBuffer::CommandBuffer(const Handle<CommandPool>& command_pool,
							 VkCommandBufferLevel level)
	: _CommandPool(command_pool, this) {
	NotifyCreation(this);

	if (_CommandPool.IsInvalid())
		throw std::runtime_error("CommandBuffer::CommandBuffer(): Command pool is null");

	VkCommandBufferAllocateInfo vk_command_buffer_allocation_info{};
	vk_command_buffer_allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	vk_command_buffer_allocation_info.commandPool = _CommandPool->GetVKCommandPool();
	vk_command_buffer_allocation_info.level = level;
	vk_command_buffer_allocation_info.commandBufferCount = 1;

	VkResult vk_result = vkAllocateCommandBuffers(_CommandPool->GetLogicalDevice()->GetVKLogicalDevice(), &vk_command_buffer_allocation_info, &_VKCommandBuffer);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("CommandBuffer::CommandBuffer(): Failed to allocate command buffer");
}

CommandBuffer::~CommandBuffer() {
	Destroy();
	NotifyDestruction(this);

	vkFreeCommandBuffers(_CommandPool->GetLogicalDevice()->GetVKLogicalDevice(), _CommandPool->GetVKCommandPool(), 1, &_VKCommandBuffer);
}

/* ---- ---- ---- ---- */

void CommandBuffer::Reset() {
    VkResult vk_result = vkResetCommandBuffer(_VKCommandBuffer, 0);
    if (vk_result != VK_SUCCESS)
        throw std::runtime_error("CommandBuffer::Reset(): Failed to reset command buffer");
}

void CommandBuffer::BeginRecording() {
    VkCommandBufferBeginInfo vk_command_buffer_begin_info{};
    vk_command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vk_command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkResult vk_result = vkBeginCommandBuffer(_VKCommandBuffer, &vk_command_buffer_begin_info);
    if (vk_result != VK_SUCCESS)
        throw std::runtime_error("CommandBuffer::BeginRecording(): Failed to begin recording command buffer");
}

void CommandBuffer::FinishRecording() {
    VkResult vk_result = vkEndCommandBuffer(_VKCommandBuffer);
    if (vk_result != VK_SUCCESS)
        throw std::runtime_error("CommandBuffer::EndRecording(): Failed to end recording command buffer");
}

void CommandBuffer::BeginRenderPass(const Handle<RenderPass>& render_pass, const Handle<Framebuffer>& framebuffer, uint32 area_width, uint32 area_height, const List<VkClearValue>& clear_values, VkSubpassContents contents) {
	if (render_pass.IsInvalid())
		throw std::runtime_error("CommandBuffer::BeginRenderPass(): Render pass is null");

	if (framebuffer.IsInvalid())
		throw std::runtime_error("CommandBuffer::BeginRenderPass(): Framebuffer is null");

	VkRenderPassBeginInfo vk_render_pass_begin_info{};
	vk_render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	vk_render_pass_begin_info.renderPass = render_pass->GetVKRenderPass();
	vk_render_pass_begin_info.framebuffer = framebuffer->GetVKFramebuffer();
	vk_render_pass_begin_info.renderArea.offset = {0, 0};
	vk_render_pass_begin_info.renderArea.extent.width = area_width;
	vk_render_pass_begin_info.renderArea.extent.height = area_height;
	vk_render_pass_begin_info.clearValueCount = clear_values.Length();
	vk_render_pass_begin_info.pClearValues = clear_values.Data();

	vkCmdBeginRenderPass(_VKCommandBuffer, &vk_render_pass_begin_info, contents);
}

void CommandBuffer::EndRenderPass() {
	vkCmdEndRenderPass(_VKCommandBuffer);
}
