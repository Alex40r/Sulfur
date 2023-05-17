#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "ICommandQueue.hpp"

class CommandQueue : public ICommandQueue, Parent<LogicalDevice>, Parent<CommandQueueFamily> {
	friend class LogicalDevice;

private:
	static Handle<CommandQueue> Create(const Handle<LogicalDevice>& logical_device,
									   const Handle<CommandQueueFamily>& command_queue_family,
									   VkQueue vk_queue,
									   uint32 queue_index) {
		return new CommandQueue(logical_device, command_queue_family, vk_queue, queue_index);
	}

private:
	CommandQueue(const Handle<LogicalDevice>& logical_device,
				 const Handle<CommandQueueFamily>& command_queue_family,
				 VkQueue vk_queue,
				 uint32 queue_index);

public:
	~CommandQueue() override;

	Handle<CommandQueue> GetCommandQueue() override { return this; }

	/* ---- ---- ---- ---- */

	/* ---- ---- ---- ---- */
private:
	VkQueue VKQueue;

	uint32 QueueID;
};