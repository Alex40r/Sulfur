#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class MemoryHeap : public Object {
	friend class PhysicalDevice;

private:
	MemoryHeap(const Handle<PhysicalDevice>& physical_device,
			   uint32 index,
			   VkMemoryHeap vk_memory_heap);

public:
	~MemoryHeap() override;

	/* ---- ---- ---- ---- */

	uint32 GetHeapID() const { return _MemoryHeapID; }
	uint64 GetHeapSize() const { return _VKMemoryHeap.size; }
	bool IsDeviceLocal() const { return _VKMemoryHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT; }

	/* ---- ---- ---- ---- */

	const ParentHandle<PhysicalDevice>& GetPhysicalDevice() const { return _PhysicalDevice; }

private:
	ParentHandle<PhysicalDevice> _PhysicalDevice;

	/* ---- ---- ---- ---- */

	uint32 _MemoryHeapID;

	VkMemoryHeap _VKMemoryHeap;
};