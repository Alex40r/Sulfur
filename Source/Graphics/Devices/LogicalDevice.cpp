#include "LogicalDevice.hpp"

#include "Graphics/CommandQueues/CommandQueue.hpp"
#include "PhysicalDevice.hpp"

LogicalDevice::CreationInfo::CreationInfo(const List<CommandQueue::CreationInfo>& command_queue_creation_infos, const List<std::string>& logical_device_extentions)
	: CommandQueueCreationInfos(command_queue_creation_infos)
	, LogicalDeviceExtentions(logical_device_extentions) {
}

LogicalDevice::LogicalDevice(const Handle<PhysicalDevice>& physical_device, const LogicalDevice::CreationInfo& creation_info)
	: Object(physical_device)
	, Info(creation_info) {
	VkResult vk_result;

	if (physical_device.IsInvalid())
		throw std::runtime_error("Invalid physical device");

	NotifyCreation(this);

	List<VkDeviceQueueCreateInfo> queue_creation_infos(Info.CommandQueueCreationInfos.GetLength());

	for (uint32 i = 0; i < queue_creation_infos.GetLength(); i++)
		Info.CommandQueueCreationInfos[i].GetVKDeviceQueueCreateInfo(queue_creation_infos[i]);

	/* TODO: Add a better way to specify device features */
	VkPhysicalDeviceFeatures device_features{};
	device_features.fillModeNonSolid = VK_TRUE;
	device_features.depthBounds = VK_TRUE;

	VkDeviceCreateInfo device_creation_info{};
	device_creation_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_creation_info.pEnabledFeatures = &device_features;
	device_creation_info.queueCreateInfoCount = queue_creation_infos.GetLength();
	device_creation_info.pQueueCreateInfos = queue_creation_infos.GetPointer();

	List<const char*> extention_names = Info.LogicalDeviceExtentions.GetCharPointerList();
	device_creation_info.enabledExtensionCount = extention_names.GetLength();
	device_creation_info.ppEnabledExtensionNames = extention_names.GetPointer();

	vk_result = vkCreateDevice(physical_device->GetVKPhysicalDevice(), &device_creation_info, nullptr, &VKLogicalDevice);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device");

	const List<Handle<CommandQueueFamily>>& queue_families = physical_device->GetCommandQueueFamilies();
	List<uint32> family_queue_indices(queue_families.GetLength());

	for (uint32 i = 0; i < family_queue_indices.GetLength(); i++)
		family_queue_indices[i] = 0;

	CommandQueues.Resize(queue_creation_infos.GetLength());

	for (uint32 i = 0; i < CommandQueues.GetLength(); i++) {
		uint32 family_id = queue_creation_infos[i].queueFamilyIndex;

		if (family_id == INVALID_ID || family_id > family_queue_indices.GetLength())
			throw std::runtime_error("Invalid family ID");

		uint32 queue_id = family_queue_indices[family_id];
		family_queue_indices[family_id]++;

		VkQueue vk_queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(VKLogicalDevice, family_id, queue_id, &vk_queue);

		if (vk_queue != VK_NULL_HANDLE)
			CommandQueues[i] = CommandQueue::Create(this,
													vk_queue,
													queue_id,
													Info.CommandQueueCreationInfos[i]);
	}
}

LogicalDevice::~LogicalDevice() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyDevice(VKLogicalDevice, nullptr);
}
