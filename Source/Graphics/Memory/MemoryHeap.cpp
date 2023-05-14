#include "MemoryHeap.hpp"

MemoryHeap::MemoryHeap(const Handle<PhysicalDevice>& physical_device, const VkMemoryHeap& vk_memory_heap, uint32 heap_id)
    : Object(physical_device)
    , VKMemoryHeap(vk_memory_heap)
    , HeapID(heap_id)
    , HeapSize(vk_memory_heap.size)
    , DeviceLocal(vk_memory_heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
    if (physical_device.IsInvalid())
        throw std::runtime_error("Invalid physical device");

    NotifyCreation(this);
}

MemoryHeap::~MemoryHeap() {
    DestroyChildren();
	NotifyDestruction(this);
}