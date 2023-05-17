#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IPhysicalDevice.hpp"

class PhysicalDevice : public IPhysicalDevice, Parent<Instance> {
	friend class Instance;

private:
	static Handle<PhysicalDevice> Create(const Handle<Instance>& instance,
										 VkPhysicalDevice vk_physical_device) {
		return new PhysicalDevice(instance, vk_physical_device);
	}

private:
	PhysicalDevice(const Handle<Instance>& instance,
				   VkPhysicalDevice vk_physical_device);

public:
	~PhysicalDevice() override;

	Handle<PhysicalDevice> GetPhysicalDevice() override { return this; }

	/* ---- ---- ---- ---- */

	VkPhysicalDevice GetVKPhysicalDevice() { return VKPhysicalDevice; }

	const List<Handle<CommandQueueFamily>>& GetCommandQueueFamilies() { return CommandQueueFamilies; }
	const Handle<CommandQueueFamily>& GetCommandQueueFamily(uint32 index) { return CommandQueueFamilies[index]; }
	uint32 GetCommandQueueFamilyCount() { return CommandQueueFamilies.GetLength(); }

	const List<Handle<MemoryHeap>>& GetMemoryHeaps() { return MemoryHeaps; }
	const Handle<MemoryHeap>& GetMemoryHeap(uint32 index) { return MemoryHeaps[index]; }
	uint32 GetMemoryHeapCount() { return MemoryHeaps.GetLength(); }

	const List<Handle<MemoryType>>& GetMemoryTypes() { return MemoryTypes; }
	const Handle<MemoryType>& GetMemoryType(uint32 index) { return MemoryTypes[index]; }
	uint32 GetMemoryTypeCount() { return MemoryTypes.GetLength(); }

	/* ---- ---- ---- ---- */
private:
	VkPhysicalDevice VKPhysicalDevice;

	VkPhysicalDeviceProperties VKProperties;
	VkPhysicalDeviceFeatures VKFeatures;
	VkPhysicalDeviceMemoryProperties VKMemoryProperties;

	List<Handle<CommandQueueFamily>> CommandQueueFamilies;
	List<Handle<MemoryHeap>> MemoryHeaps;
	List<Handle<MemoryType>> MemoryTypes;
};