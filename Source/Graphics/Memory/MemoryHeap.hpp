#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IMemoryHeap.hpp"

class MemoryHeap : public IMemoryHeap, Parent<PhysicalDevice> {
    friend class PhysicalDevice;

private:
	static Handle<MemoryHeap> Create(const Handle<PhysicalDevice>& physical_device,
									 const VkMemoryHeap& vk_memory_heap,
									 uint32 heap_id) {
		return new MemoryHeap(physical_device, vk_memory_heap, heap_id);
	}

private:
	MemoryHeap(const Handle<PhysicalDevice>& physical_device,
			   const VkMemoryHeap& vk_memory_heap,
			   uint32 heap_id);

public:
	~MemoryHeap() override;

	Handle<MemoryHeap> GetMemoryHeap() override { return this; }

	/* ---- ---- ---- ---- */

	uint32 GetHeapID() const { return HeapID; }
	uint64 GetHeapSize() const { return HeapSize; }
	bool IsDeviceLocal() const { return DeviceLocal; }

	/* ---- ---- ---- ---- */
private:
	VkMemoryHeap VKMemoryHeap;
	uint32 HeapID;
	uint64 HeapSize;
	bool DeviceLocal = false;
};