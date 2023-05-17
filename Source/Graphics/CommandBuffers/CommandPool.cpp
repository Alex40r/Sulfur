#include "CommandPool.hpp"

#include "Graphics/CommandQueues/CommandQueueFamily.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"

CommandPool::CommandPool(const Handle<LogicalDevice>& logical_device,
									 const Handle<CommandQueueFamily>& command_queue_family,
									 VkCommandPoolCreateFlags flags)
	: Parent<LogicalDevice>(logical_device)
	, Parent<CommandQueueFamily>(command_queue_family) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("CommandBufferPool must be created with a valid LogicalDevice");

	if (Parent<CommandQueueFamily>::Get().IsInvalid())
		throw std::runtime_error("CommandBufferPool must be created with a valid CommandQueueFamily");

	VkResult vk_result;

	VkCommandPoolCreateInfo pool_creation_info{};
	pool_creation_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_creation_info.flags = flags;
	pool_creation_info.queueFamilyIndex = command_queue_family->GetFamilyID();

	vk_result = vkCreateCommandPool(Parent<LogicalDevice>::Get()->GetVKLogicalDevice(), &pool_creation_info, nullptr, &VKCommandPool);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create command buffer pool");
}

CommandPool::~CommandPool() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyCommandPool(Parent<LogicalDevice>::Get()->GetVKLogicalDevice(), VKCommandPool, nullptr);
}

/* ---- ---- ---- ---- */