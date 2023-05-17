#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "ICommandPool.hpp"

class CommandPool : public ICommandPool, Parent<LogicalDevice>, Parent<CommandQueueFamily> {
public:
	static Handle<CommandPool> Create(const Handle<LogicalDevice>& logical_device,
											const Handle<CommandQueueFamily>& command_queue_family,
											VkCommandPoolCreateFlags flags = 0) {
		return new CommandPool(logical_device, command_queue_family, flags);
	}

private:
	CommandPool(const Handle<LogicalDevice>& logical_device,
					  const Handle<CommandQueueFamily>& command_queue_family,
					  VkCommandPoolCreateFlags flags);

public:
	~CommandPool() override;

	Handle<CommandPool> GetCommandBufferPool() override { return this; }

	/* ---- ---- ---- ---- */

	Handle<LogicalDevice> GetLogicalDevice() { return Parent<LogicalDevice>::Get(); }

	VkCommandPool GetVKCommandPool() { return VKCommandPool; }

	/* ---- ---- ---- ---- */
private:
	VkCommandPool VKCommandPool;
};