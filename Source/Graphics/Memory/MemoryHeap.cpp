#include "MemoryHeap.hpp"

MemoryHeap::MemoryHeap(const Handle<PhysicalDevice>& physical_device,
					   const VkMemoryHeap& vk_memory_heap,
					   uint32 heap_id)
	: Parent<PhysicalDevice>(physical_device)
	, VKMemoryHeap(vk_memory_heap)
	, HeapID(heap_id)
	, HeapSize(vk_memory_heap.size)
	, DeviceLocal(vk_memory_heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
	NotifyCreation(this);

    if (Parent<PhysicalDevice>::Get().IsInvalid())
        throw std::runtime_error("MemoryHeap must be created with a valid PhysicalDevice");
}

MemoryHeap::~MemoryHeap() {
	DestroyChildren();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */