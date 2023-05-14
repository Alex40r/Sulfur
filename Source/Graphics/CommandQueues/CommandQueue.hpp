#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class CommandQueue : public Object<LogicalDevice, CommandQueueFamily> {
	friend class LogicalDevice;

public:
	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(uint32 family_id,
					 float priority);
		CreationInfo(const Handle<CommandQueueFamily>& family,
					 float priority);

		uint32 FamilyID = INVALID_ID;
		float Priority = 1.0F;

		void GetVKDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& vk_device_queue_create_info);
	};

private:
	static Handle<CommandQueue> Create(const Handle<LogicalDevice>& logical_device,
									   VkQueue vk_queue,
									   uint32 queue_id,
									   const CommandQueue::CreationInfo& creation_info) {
		return new CommandQueue(logical_device, vk_queue, queue_id, creation_info);
	}

	CommandQueue(const Handle<LogicalDevice>& logical_device,
				 VkQueue vk_queue,
				 uint32 queue_id,
				 const CommandQueue::CreationInfo& creation_info);

	static const Handle<CommandQueueFamily>& GetQueueFamily(const Handle<LogicalDevice>& logical_device,
													 uint32 family_id);

public:
	~CommandQueue();

	const CommandQueue::CreationInfo& GetCreationInfo() { return Info; }

private:
	CommandQueue::CreationInfo Info;

	VkQueue VKQueue;

	uint32 QueueID;
};