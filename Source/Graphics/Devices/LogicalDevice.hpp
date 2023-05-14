#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

#include "Graphics/CommandQueues/CommandQueue.hpp"

class LogicalDevice : public Object<PhysicalDevice> {
public:
	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(const List<CommandQueue::CreationInfo>& command_queue_creation_infos,
					 const List<std::string>& logical_device_extentions);

		List<CommandQueue::CreationInfo> CommandQueueCreationInfos;
		List<std::string> LogicalDeviceExtentions;
	};

	static Handle<LogicalDevice> Create(const Handle<PhysicalDevice>& physical_device,
										const List<CommandQueue::CreationInfo>& command_queue_creation_infos,
										const List<std::string>& logical_device_extentions) {
		return new LogicalDevice(physical_device, LogicalDevice::CreationInfo(command_queue_creation_infos, logical_device_extentions));
	}
	static Handle<LogicalDevice> Create(const Handle<PhysicalDevice>& physical_device,
										const LogicalDevice::CreationInfo& creation_info) {
		return new LogicalDevice(physical_device, creation_info);
	}

private:
	LogicalDevice(const Handle<PhysicalDevice>& physical_device,
				  const LogicalDevice::CreationInfo& creation_info);

public:
	~LogicalDevice();

	const LogicalDevice::CreationInfo& GetCreationInfo() { return Info; }

	const List<Handle<CommandQueue>>& GetCommandQueues() const { return CommandQueues; }

	VkDevice GetVKLogicalDevice() { return VKLogicalDevice; }

private:
	LogicalDevice::CreationInfo Info;

	VkDevice VKLogicalDevice;

	List<Handle<CommandQueue>> CommandQueues;
};