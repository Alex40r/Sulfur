#include "CommandBuffer.hpp"

#include "CommandPool.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"

CommandBuffer::CommandBuffer(const Handle<CommandPool> command_pool, VkCommandBufferLevel level)
	: Parent<CommandPool>(command_pool) {
	NotifyCreation(this);

	if (Parent<CommandPool>::Get().IsInvalid())
		throw std::runtime_error("CommandBuffer must be created with a valid CommandBufferPool");

	VkResult vk_result;

	VkCommandBufferAllocateInfo allocation_info{};
	allocation_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocation_info.commandPool = command_pool->GetVKCommandPool();
	allocation_info.level = level;
	allocation_info.commandBufferCount = 1;

	vk_result = vkAllocateCommandBuffers(command_pool->GetLogicalDevice()->GetVKLogicalDevice(), &allocation_info, &VKCommandBuffer);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffer");
}

CommandBuffer::~CommandBuffer() {
	DestroyChildren();
	NotifyDestruction(this);

    vkFreeCommandBuffers(Parent<CommandPool>::Get()->GetLogicalDevice()->GetVKLogicalDevice(), Parent<CommandPool>::Get()->GetVKCommandPool(), 1, &VKCommandBuffer);
}

/* ---- ---- ---- ---- */