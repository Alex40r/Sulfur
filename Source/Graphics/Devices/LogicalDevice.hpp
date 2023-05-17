#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "ILogicalDevice.hpp"

struct CommandQueueDescriptor {
	CommandQueueDescriptor() = default;
	CommandQueueDescriptor(uint32 family_id,
						   float priority);
	CommandQueueDescriptor(const Handle<CommandQueueFamily>& family,
						   float priority);

	void GetVKDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& vk_queue_creation_info) const;

	uint32 FamilyID = INVALID_ID;
	float Priority = 1.0F;
};

class LogicalDevice : public ILogicalDevice, Parent<PhysicalDevice> {
public:
	static Handle<LogicalDevice> Create(const Handle<PhysicalDevice>& physical_device,
										const List<std::string>& extensions,
										const List<CommandQueueDescriptor> command_queue_descriptors) {
		return new LogicalDevice(physical_device, extensions, command_queue_descriptors);
	}

private:
	LogicalDevice(const Handle<PhysicalDevice>& physical_device,
				  const List<std::string>& extensions,
				  const List<CommandQueueDescriptor> command_queue_descriptors);

public:
	~LogicalDevice() override;

	Handle<LogicalDevice> GetLogicalDevice() override { return this; }

	/* ---- ---- ---- ---- */

	const List<Handle<CommandQueue>>& GetCommandQueues() { return CommandQueues; }
	const Handle<CommandQueue>& GetCommandQueue(uint32 index) { return CommandQueues[index]; }
	uint32 GetCommandQueueCount() { return CommandQueues.GetLength(); }

	VkDevice GetVKLogicalDevice() { return VKLogicalDevice; }

	/* ---- ---- ---- ---- */
private:
	VkDevice VKLogicalDevice;

	List<Handle<CommandQueue>> CommandQueues;
};