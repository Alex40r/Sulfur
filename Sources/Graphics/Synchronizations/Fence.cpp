#include "Fence.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"

Fence::Fence(const Handle<LogicalDevice>& logical_device,
			 bool signaled)
	: _LogicalDevice(logical_device, this) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("Fence::Fence(): Logical device is null");

	VkFenceCreateInfo vk_fence_creation_info{};
	vk_fence_creation_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vk_fence_creation_info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	VkResult vk_result = vkCreateFence(_LogicalDevice->GetVKLogicalDevice(), &vk_fence_creation_info, nullptr, &_VKFence);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Fence::Fence(): Failed to create fence");
}


Fence::~Fence() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyFence(_LogicalDevice->GetVKLogicalDevice(), _VKFence, nullptr);
}

/* ---- ---- ---- ---- */


void Fence::Wait(uint64 timeout) {
    VkResult vk_result = vkWaitForFences(_LogicalDevice->GetVKLogicalDevice(), 1, &_VKFence, VK_TRUE, timeout);
    if (vk_result != VK_SUCCESS)
        throw std::runtime_error("Fence::Wait(): Failed to wait for fence");
}

void Fence::Reset() {
    VkResult vk_result = vkResetFences(_LogicalDevice->GetVKLogicalDevice(), 1, &_VKFence);
    if (vk_result != VK_SUCCESS)
        throw std::runtime_error("Fence::Reset(): Failed to reset fence");
}
