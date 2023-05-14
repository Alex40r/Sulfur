#include "CommandQueueFamily.hpp"

CommandQueueFamily::CommandQueueFamily(const Handle<PhysicalDevice>& physical_device, const VkQueueFamilyProperties& vk_queue_family_properties, uint32 index)
	: Object(physical_device)
	, VKQueueFamilyProperties(vk_queue_family_properties)
	, Index(index) {
	if (physical_device.IsInvalid())
		throw std::runtime_error("Invalid physical device");

	NotifyCreation(this);

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		SupportedFeatures |= QUEUE_FEATURE_FLAG_GRAPHICS;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		SupportedFeatures |= QUEUE_FEATURE_FLAG_COMPUTE;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		SupportedFeatures |= QUEUE_FEATURE_FLAG_TRANSFER;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
		SupportedFeatures |= QUEUE_FEATURE_FLAG_SPARSE_BINDING;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_PROTECTED_BIT)
		SupportedFeatures |= QUEUE_FEATURE_FLAG_PROTECTED_MEMORY;

	if (vk_queue_family_properties.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
		SupportedFeatures |= QUEUE_FEATURE_FLAG_OPTICAL_FLOW;
}

CommandQueueFamily::~CommandQueueFamily() {
	DestroyChildren();
	NotifyDestruction(this);
}