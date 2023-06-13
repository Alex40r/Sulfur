#include "Semaphore.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"

Semaphore::Semaphore(const Handle<LogicalDevice>& logical_device)
	: _LogicalDevice(logical_device, this) {
	NotifyCreation(this);

    if(_LogicalDevice.IsInvalid())
        throw std::runtime_error("Semaphore::Semaphore(): Logical device is null");

    VkSemaphoreCreateInfo vk_semaphore_creation_info{};
    vk_semaphore_creation_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult vk_result = vkCreateSemaphore(_LogicalDevice->GetVKLogicalDevice(), &vk_semaphore_creation_info, nullptr, &_VKSemaphore);
    if(vk_result != VK_SUCCESS)
        throw std::runtime_error("Semaphore::Semaphore(): Failed to create semaphore");
}

Semaphore::~Semaphore() {
	Destroy();
	NotifyDestruction(this);

    vkDestroySemaphore(_LogicalDevice->GetVKLogicalDevice(), _VKSemaphore, nullptr);
}

/* ---- ---- ---- ---- */