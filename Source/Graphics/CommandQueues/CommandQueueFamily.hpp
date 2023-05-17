#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "ICommandQueueFamily.hpp"

typedef uint32 QueueFeatureFlags;
enum QueueFeatureFlagsBit : QueueFeatureFlags {
	QUEUE_FEATURE_FLAG_NONE = 0U,

	QUEUE_FEATURE_FLAG_GRAPHICS = 1U << 0,
	QUEUE_FEATURE_FLAG_COMPUTE = 1U << 1,
	QUEUE_FEATURE_FLAG_TRANSFER = 1U << 2,
	QUEUE_FEATURE_FLAG_SPARSE_BINDING = 1U << 3,
	QUEUE_FEATURE_FLAG_PROTECTED_MEMORY = 1U << 4,
	QUEUE_FEATURE_FLAG_OPTICAL_FLOW = 1U << 5,
	QUEUE_FEATURE_FLAG_PRESENT = 1U << 6,
};

class CommandQueueFamily : public ICommandQueueFamily, Parent<PhysicalDevice> {
	friend class PhysicalDevice;

private:
	static Handle<CommandQueueFamily> Create(const Handle<PhysicalDevice>& physical_device,
											 const VkQueueFamilyProperties& vk_queue_family_properties,
											 uint32 family_index) {
		return new CommandQueueFamily(physical_device, vk_queue_family_properties, family_index);
	}

private:
	CommandQueueFamily(const Handle<PhysicalDevice>& physical_device,
					   const VkQueueFamilyProperties& vk_queue_family_properties,
					   uint32 family_index);

public:
	~CommandQueueFamily() override;

	Handle<CommandQueueFamily> GetCommandQueueFamily() override { return this; }

	/* ---- ---- ---- ---- */

    uint32 GetFamilyID() { return Index; }

	/* ---- ---- ---- ---- */
private:
	VkQueueFamilyProperties VKQueueFamilyProperties;
	uint32 Index;
	QueueFeatureFlags SupportedFeatures = QUEUE_FEATURE_FLAG_NONE;
};