#include "PhysicalDevice.hpp"

#include "Graphics/CommandQueues/CommandQueueFamily.hpp"
#include "Graphics/Memory/MemoryHeap.hpp"
#include "Graphics/Memory/MemoryType.hpp"

PhysicalDevice::PhysicalDevice(const Handle<Instance>& instance, VkPhysicalDevice vk_physical_device)
	: Object(instance)
	, VKPhysicalDevice(vk_physical_device) {
    if (instance.IsInvalid())
        throw std::runtime_error("Invalid instance");

	NotifyCreation(this);

	vkGetPhysicalDeviceProperties(vk_physical_device, &VKProperties);
	vkGetPhysicalDeviceFeatures(vk_physical_device, &VKFeatures);
	vkGetPhysicalDeviceMemoryProperties(vk_physical_device, &VKMemoryProperties);

	uint32 family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &family_count, nullptr);

	List<VkQueueFamilyProperties> vk_family_properties(family_count);

	vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &family_count, vk_family_properties.GetPointer());

	CommandQueueFamilies.Resize(family_count);
	for (uint32 i = 0; i < family_count; i++)
		CommandQueueFamilies[i] = CommandQueueFamily::Create(this, vk_family_properties[i], i);

	MemoryHeaps.Resize(VKMemoryProperties.memoryHeapCount);
	for (uint32 i = 0; i < VKMemoryProperties.memoryHeapCount; i++)
		MemoryHeaps[i] = MemoryHeap::Create(this, VKMemoryProperties.memoryHeaps[i], i);

	MemoryTypes.Resize(VKMemoryProperties.memoryTypeCount);
	for (uint32 i = 0; i < VKMemoryProperties.memoryTypeCount; i++)
		MemoryTypes[i] = MemoryType::Create(MemoryHeaps[VKMemoryProperties.memoryTypes[i].heapIndex], VKMemoryProperties.memoryTypes[i], i);
}

PhysicalDevice::~PhysicalDevice() {
	DestroyChildren();
	NotifyDestruction(this);
}
