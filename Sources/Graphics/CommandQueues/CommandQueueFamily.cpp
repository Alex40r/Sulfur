#include "CommandQueueFamily.hpp"

#include "Graphics/Devices/PhysicalDevice.hpp"

CommandQueueFamily::CommandQueueFamily(const Handle<PhysicalDevice>& physical_device,
									   uint32 index,
									   VkQueueFamilyProperties vk_queue_family_properties)
	: _PhysicalDevice(physical_device, this)
	, _CommandQueueFamilyID(index)
	, _VKQueueFamilyProperties(vk_queue_family_properties) {
	NotifyCreation(this);

	if (_PhysicalDevice.IsInvalid())
		throw std::runtime_error("CommandQueueFamily::CommandQueueFamily(): Physical device is null");

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		_SupportedFeatures |= QUEUE_FEATURE_FLAG_GRAPHICS;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		_SupportedFeatures |= QUEUE_FEATURE_FLAG_COMPUTE;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		_SupportedFeatures |= QUEUE_FEATURE_FLAG_TRANSFER;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
		_SupportedFeatures |= QUEUE_FEATURE_FLAG_SPARSE_BINDING;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_PROTECTED_BIT)
		_SupportedFeatures |= QUEUE_FEATURE_FLAG_PROTECTED_MEMORY;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
		_SupportedFeatures |= QUEUE_FEATURE_FLAG_OPTICAL_FLOW;
}

CommandQueueFamily::~CommandQueueFamily() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */