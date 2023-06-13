#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class CommandPool : public Object {
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

	/* ---- ---- ---- ---- */

	VkCommandPool GetVKCommandPool() const { return VKCommandPool; }

	void Reset();

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<CommandQueueFamily>& GetCommandQueueFamily() const { return _CommandQueueFamily; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<CommandQueueFamily> _CommandQueueFamily;

	/* ---- ---- ---- ---- */

	VkCommandPool VKCommandPool;
};