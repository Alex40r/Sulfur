#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class PhysicalDevice : public Object<Instance> {
	friend class Instance;

private:
	static Handle<PhysicalDevice> Create(const Handle<Instance>& instance,
										 VkPhysicalDevice vk_physical_device) {
		return new PhysicalDevice(instance, vk_physical_device);
	}

	PhysicalDevice(const Handle<Instance>& instance,
				   VkPhysicalDevice vk_physical_device);

public:
	~PhysicalDevice();

	const List<Handle<CommandQueueFamily>>& GetCommandQueueFamilies() const { return CommandQueueFamilies; }
	const List<Handle<MemoryHeap>>& GetMemoryHeaps() const { return MemoryHeaps; }
	const List<Handle<MemoryType>>& GetMemoryTypes() const { return MemoryTypes; }

	VkPhysicalDevice GetVKPhysicalDevice() const { return VKPhysicalDevice; } 

private:
	VkPhysicalDevice VKPhysicalDevice;

	VkPhysicalDeviceProperties VKProperties;
	VkPhysicalDeviceFeatures VKFeatures;
	VkPhysicalDeviceMemoryProperties VKMemoryProperties;

	List<Handle<CommandQueueFamily>> CommandQueueFamilies;
	List<Handle<MemoryHeap>> MemoryHeaps;
	List<Handle<MemoryType>> MemoryTypes;
};