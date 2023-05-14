#include "CommandQueue.hpp"

#include "CommandQueueFamily.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"

CommandQueue::CreationInfo::CreationInfo(uint32 family_id, float priority)
	: FamilyID(family_id)
	, Priority(priority) {
}

CommandQueue::CreationInfo::CreationInfo(const Handle<CommandQueueFamily>& family, float priority)
	: CreationInfo(family->GetFamilyIndex(), priority) {
}

void CommandQueue::CreationInfo::GetVKDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& vk_device_queue_create_info) {
	vk_device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vk_device_queue_create_info.flags = 0;
	vk_device_queue_create_info.pNext = nullptr;
	vk_device_queue_create_info.queueFamilyIndex = FamilyID;
	vk_device_queue_create_info.queueCount = 1;
	vk_device_queue_create_info.pQueuePriorities = &Priority;
}

CommandQueue::CommandQueue(const Handle<LogicalDevice>& logical_device,
						   VkQueue vk_queue,
						   uint32 queue_id,
						   const CommandQueue::CreationInfo& creation_info)
	: Object(logical_device, GetQueueFamily(logical_device, creation_info.FamilyID))
	, Info(creation_info)
	, VKQueue(vk_queue)
	, QueueID(queue_id) {
	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	if (Utils::GetParent<CommandQueueFamily>(this).IsInvalid())
		throw std::runtime_error("Invalid queue family");

	if (VKQueue == VK_NULL_HANDLE)
		throw std::runtime_error("Invalid command queue");

	NotifyCreation(this);
}

const Handle<CommandQueueFamily>& CommandQueue::GetQueueFamily(const Handle<LogicalDevice>& logical_device, uint32 family_id) {
	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	if (family_id == INVALID_ID)
		throw std::runtime_error("Invalid queue family ID");

	const List<Handle<CommandQueueFamily>>& command_queue_families = Utils::GetParent<PhysicalDevice>(logical_device)->GetCommandQueueFamilies();

	if (family_id >= command_queue_families.GetLength())
		throw std::runtime_error("Invalid queue family ID");

	return command_queue_families[family_id];
}

CommandQueue::~CommandQueue() {
	DestroyChildren();
	NotifyDestruction(this);
}
