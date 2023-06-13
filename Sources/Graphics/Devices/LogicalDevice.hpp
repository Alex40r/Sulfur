#pragma once

#include "Utils/Utils.hpp"

#include "Graphics/CommandQueues/CommandQueue.hpp"

class LogicalDevice : public Object {
public:
	static Handle<LogicalDevice> Create(const Handle<PhysicalDevice>& physical_device,
										const List<std::string>& extensions,
										const List<CommandQueue::CreationInfo> queue_creation_infos) {
		return new LogicalDevice(physical_device, extensions, queue_creation_infos);
	}

private:
	LogicalDevice(const Handle<PhysicalDevice>& physical_device,
				  const List<std::string>& extensions,
				  const List<CommandQueue::CreationInfo> queue_creation_infos);

public:
	~LogicalDevice() override;

	/* ---- ---- ---- ---- */

	VkDevice GetVKLogicalDevice() const { return _VKLogicalDevice; }

	const List<Handle<CommandQueue>>& GetCommandQueues() const { return _CommandQueues; }
	const Handle<CommandQueue>& GetCommandQueue(uint32 index) const { return _CommandQueues[index]; }
	uint32 GetCommandQueueCount() const { return _CommandQueues.Length(); }

	/* ---- ---- ---- ---- */

	const ParentHandle<PhysicalDevice>& GetPhysicalDevice() const { return _PhysicalDevice; }

private:
	ParentHandle<PhysicalDevice> _PhysicalDevice;

	/* ---- ---- ---- ---- */

	VkDevice _VKLogicalDevice;

	List<Handle<CommandQueue>> _CommandQueues;
};