#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class PhysicalDevice : public Object {
	friend class Instance;

private:
	PhysicalDevice(const Handle<Instance>& instance,
				   uint32 index,
				   VkPhysicalDevice vk_physical_device);

public:
	~PhysicalDevice() override;

	/* ---- ---- ---- ---- */

	VkPhysicalDevice GetVKPhysicalDevice() const { return _VKPhysicalDevice; }

    uint32 GetPhysicalDeviceID() const { return _PhysicalDeviceID; }

	const List<Handle<CommandQueueFamily>>& GetCommandQueueFamilies() const { return _CommandQueueFamilies; }
	const Handle<CommandQueueFamily>& GetCommandQueueFamily(uint32 index) const { return _CommandQueueFamilies[index]; }
	uint32 GetCommandQueueFamilyCount() const { return _CommandQueueFamilies.Length(); }

	const List<Handle<MemoryHeap>>& GetMemoryHeaps() const { return _MemoryHeaps; }
	const Handle<MemoryHeap>& GetMemoryHeap(uint32 index) const { return _MemoryHeaps[index]; }
	uint32 GetMemoryHeapCount() const { return _MemoryHeaps.Length(); }

	const List<Handle<MemoryType>>& GetMemoryTypes() const { return _MemoryTypes; }
	const Handle<MemoryType>& GetMemoryType(uint32 index) const { return _MemoryTypes[index]; }
	uint32 GetMemoryTypeCount() const { return _MemoryTypes.Length(); }

	const Handle<MemoryType>& FindMemoryType(uint32 memory_types, AllocationType allocation_type, VkMemoryPropertyFlags target_properties = 0) const;

	/* ---- ---- ---- ---- */

	const ParentHandle<Instance>& GetInstance() const { return _Instance; }

private:
	ParentHandle<Instance> _Instance;

	/* ---- ---- ---- ---- */

	uint32 FindLargestMemoryTypeID(uint32 memory_types, VkMemoryPropertyFlags target_properties) const;

	uint32 _PhysicalDeviceID;

	VkPhysicalDevice _VKPhysicalDevice;

	VkPhysicalDeviceProperties _VKProperties;
	VkPhysicalDeviceFeatures _VKFeatures;
	VkPhysicalDeviceMemoryProperties _VKMemoryProperties;

	List<Handle<CommandQueueFamily>> _CommandQueueFamilies;
	List<Handle<MemoryHeap>> _MemoryHeaps;
	List<Handle<MemoryType>> _MemoryTypes;
};