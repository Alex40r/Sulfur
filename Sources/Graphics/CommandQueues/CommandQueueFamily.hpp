#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

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

class CommandQueueFamily : public Object {
	friend class PhysicalDevice;

private:
	CommandQueueFamily(const Handle<PhysicalDevice>& physical_device,
					   uint32 index,
					   VkQueueFamilyProperties vk_queue_family_properties);

public:
	~CommandQueueFamily() override;

	/* ---- ---- ---- ---- */

    uint32 GetFamilyID() const { return _CommandQueueFamilyID; }
    uint32 GetCapacity() const { return _VKQueueFamilyProperties.queueCount; }

	/* ---- ---- ---- ---- */

	const ParentHandle<PhysicalDevice>& GetPhysicalDevice() const { return _PhysicalDevice; }

private:
	ParentHandle<PhysicalDevice> _PhysicalDevice;

	/* ---- ---- ---- ---- */

	uint32 _CommandQueueFamilyID;

	VkQueueFamilyProperties _VKQueueFamilyProperties;
	QueueFeatureFlags _SupportedFeatures = QUEUE_FEATURE_FLAG_NONE;
};