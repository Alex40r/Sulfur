#include "MemoryHeap.hpp"

MemoryHeap::MemoryHeap(const Handle<PhysicalDevice>& physical_device,
					   uint32 index,
					   VkMemoryHeap vk_memory_heap)
	: _PhysicalDevice(physical_device, this)
	, _MemoryHeapID(index)
	, _VKMemoryHeap(vk_memory_heap) {
	NotifyCreation(this);

	if (_PhysicalDevice.IsInvalid())
		throw std::runtime_error("MemoryHeap::MemoryHeap(): Physical device is null");
}

MemoryHeap::~MemoryHeap() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */