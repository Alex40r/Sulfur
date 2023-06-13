#include "LogicalDevice.hpp"

#include "Graphics/CommandQueues/CommandQueue.hpp"
#include "PhysicalDevice.hpp"

LogicalDevice::LogicalDevice(const Handle<PhysicalDevice>& physical_device,
							 const List<std::string>& extensions,
							 const List<CommandQueue::CreationInfo> queue_creation_infos)
	: _PhysicalDevice(physical_device, this) {
	NotifyCreation(this);

	if (_PhysicalDevice.IsInvalid())
		throw std::runtime_error("LogicalDevice::LogicalDevice(): Physical device is null");

	List<VkDeviceQueueCreateInfo> vk_queue_creation_infos(queue_creation_infos.Length());

	for (uint32 i = 0; i < queue_creation_infos.Length(); i++)
		queue_creation_infos[i].GetVKQueueCreationInfo(vk_queue_creation_infos[i]);

	/* TODO: Add a better way to specify device features */
	VkPhysicalDeviceFeatures vk_device_features{};
	vk_device_features.fillModeNonSolid = VK_TRUE;
	vk_device_features.depthBounds = VK_TRUE;

	List<const char*> vk_extensions(extensions.Length());
	for (uint32 i = 0; i < extensions.Length(); i++)
		vk_extensions[i] = extensions[i].c_str();

	VkDeviceCreateInfo vk_device_creation_info{};
	vk_device_creation_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vk_device_creation_info.pEnabledFeatures = &vk_device_features;
	vk_device_creation_info.queueCreateInfoCount = vk_queue_creation_infos.Length();
	vk_device_creation_info.pQueueCreateInfos = vk_queue_creation_infos.Data();
	vk_device_creation_info.enabledExtensionCount = vk_extensions.Length();
	vk_device_creation_info.ppEnabledExtensionNames = vk_extensions.Data();

	VkResult vk_result = vkCreateDevice(_PhysicalDevice->GetVKPhysicalDevice(), &vk_device_creation_info, nullptr, &_VKLogicalDevice);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("LogicalDevice::LogicalDevice(): Failed to create logical device");

	_CommandQueues.Resize(queue_creation_infos.Length());
	List<uint32> queue_family_indices(queue_creation_infos.Length());

	for (uint32 i = 0; i < queue_creation_infos.Length(); i++) {
		uint32 queue_family_id = queue_creation_infos[i].FamilyID;
		uint32 queue_id = queue_family_indices[queue_family_id];
		queue_family_indices[queue_family_id]++;

		VkQueue vk_queue;
		vkGetDeviceQueue(_VKLogicalDevice, queue_family_id, queue_id, &vk_queue);

		if (vk_queue == VK_NULL_HANDLE)
			throw std::runtime_error("LogicalDevice::LogicalDevice(): Failed to query command queue");

		_CommandQueues[i] = new CommandQueue(this, _PhysicalDevice->GetCommandQueueFamily(queue_family_id), queue_id, vk_queue);
	}
}

LogicalDevice::~LogicalDevice() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyDevice(_VKLogicalDevice, nullptr);
}

/* ---- ---- ---- ---- */