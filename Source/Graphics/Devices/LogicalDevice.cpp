#include "LogicalDevice.hpp"

#include "Graphics/CommandQueues/CommandQueue.hpp"
#include "Graphics/CommandQueues/CommandQueueFamily.hpp"
#include "PhysicalDevice.hpp"

CommandQueueDescriptor::CommandQueueDescriptor(uint32 family_id, float priority)
	: FamilyID(family_id)
	, Priority(priority) {
}

CommandQueueDescriptor::CommandQueueDescriptor(const Handle<CommandQueueFamily>& family, float priority)
	: FamilyID(family->GetFamilyID())
	, Priority(priority) {
}

void CommandQueueDescriptor::GetVKDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& vk_queue_creation_info) const {
	vk_queue_creation_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vk_queue_creation_info.flags = 0;
	vk_queue_creation_info.pNext = nullptr;
	vk_queue_creation_info.queueFamilyIndex = FamilyID;
	vk_queue_creation_info.queueCount = 1;
	vk_queue_creation_info.pQueuePriorities = &Priority;
}

/* ---- ---- ---- ---- */

LogicalDevice::LogicalDevice(const Handle<PhysicalDevice>& physical_device,
							 const List<std::string>& extensions,
							 const List<CommandQueueDescriptor> command_queue_descriptors)
	: Parent<PhysicalDevice>(physical_device) {
	NotifyCreation(this);

	if (Parent<PhysicalDevice>::Get().IsInvalid())
		throw std::runtime_error("LogicalDevice must be created with a valid PhysicalDevice");

	VkResult vk_result;

	List<VkDeviceQueueCreateInfo> command_queue_creation_info(command_queue_descriptors.GetLength());

	for (uint32 i = 0; i < command_queue_creation_info.GetLength(); i++)
		command_queue_descriptors[i].GetVKDeviceQueueCreateInfo(command_queue_creation_info[i]);

	/* TODO: Add a better way to specify device features */
	VkPhysicalDeviceFeatures device_features{};
	device_features.fillModeNonSolid = VK_TRUE;
	device_features.depthBounds = VK_TRUE;

	VkDeviceCreateInfo device_creation_info{};
	device_creation_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_creation_info.pEnabledFeatures = &device_features;
	device_creation_info.queueCreateInfoCount = command_queue_creation_info.GetLength();
	device_creation_info.pQueueCreateInfos = command_queue_creation_info.GetPointer();

	List<const char*> vk_extensions(extensions.GetLength());
	for (uint32 i = 0; i < vk_extensions.GetLength(); i++)
		vk_extensions[i] = extensions[i].c_str();

	device_creation_info.enabledExtensionCount = vk_extensions.GetLength();
	device_creation_info.ppEnabledExtensionNames = vk_extensions.GetPointer();

	vk_result = vkCreateDevice(physical_device->GetVKPhysicalDevice(), &device_creation_info, nullptr, &VKLogicalDevice);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device");

	List<uint32> family_queue_indices(physical_device->GetCommandQueueFamilyCount());

	CommandQueues.Resize(command_queue_descriptors.GetLength());

	for (uint32 i = 0; i < CommandQueues.GetLength(); i++) {
		uint32 family_id = command_queue_creation_info[i].queueFamilyIndex;

		if (family_id == INVALID_ID || family_id > family_queue_indices.GetLength())
			throw std::runtime_error("Invalid family ID");

		uint32 queue_id = family_queue_indices[family_id];
		family_queue_indices[family_id]++;

		VkQueue vk_queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(VKLogicalDevice, family_id, queue_id, &vk_queue);

		if (vk_queue == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create command queue");

		if (vk_queue != VK_NULL_HANDLE)
			CommandQueues[i] = CommandQueue::Create(this, physical_device->GetCommandQueueFamily(family_id), vk_queue, queue_id);
	}
}

LogicalDevice::~LogicalDevice() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyDevice(VKLogicalDevice, nullptr);
}

/* ---- ---- ---- ---- */
