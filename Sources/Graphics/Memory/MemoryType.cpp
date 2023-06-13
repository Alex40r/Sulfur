#include "MemoryType.hpp"

#include "Graphics/Devices/PhysicalDevice.hpp"

MemoryType::MemoryType(const Handle<MemoryHeap>& memory_heap,
					   uint32 index,
					   VkMemoryType vk_memory_type)
	: _MemoryHeap(memory_heap, this)
	, _MemoryTypeID(index)
	, _VKMemoryType(vk_memory_type) {
	NotifyCreation(this);

	if (_MemoryHeap.IsInvalid())
		throw std::runtime_error("MemoryType::MemoryType(): Memory heap is null");
}

MemoryType::~MemoryType() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */