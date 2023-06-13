#include "CommandPool.hpp"

#include "Graphics/CommandQueues/CommandQueueFamily.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"

CommandPool::CommandPool(const Handle<LogicalDevice>& logical_device,
						 const Handle<CommandQueueFamily>& command_queue_family,
						 VkCommandPoolCreateFlags flags)
	: _LogicalDevice(logical_device, this)
	, _CommandQueueFamily(command_queue_family, this) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("CommandPool::CommandPool(): Logical device is null");

	if (_CommandQueueFamily.IsInvalid())
		throw std::runtime_error("CommandPool::CommandPool(): Command queue family is null");

	VkCommandPoolCreateInfo vk_command_pool_creation_info{};
	vk_command_pool_creation_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vk_command_pool_creation_info.queueFamilyIndex = _CommandQueueFamily->GetFamilyID();
	vk_command_pool_creation_info.flags = flags;

	VkResult vk_result = vkCreateCommandPool(_LogicalDevice->GetVKLogicalDevice(), &vk_command_pool_creation_info, nullptr, &VKCommandPool);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("CommandPool::CommandPool(): Failed to create command pool");
}

CommandPool::~CommandPool() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyCommandPool(_LogicalDevice->GetVKLogicalDevice(), VKCommandPool, nullptr);
}

/* ---- ---- ---- ---- */

void CommandPool::Reset() {
	VkResult vk_result = vkResetCommandPool(_LogicalDevice->GetVKLogicalDevice(), VKCommandPool, 0);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("CommandPool::Reset(): Failed to reset command pool");
}