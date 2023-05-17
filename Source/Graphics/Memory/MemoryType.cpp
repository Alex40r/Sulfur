#include "MemoryType.hpp"

#include "MemoryHeap.hpp"

MemoryType::MemoryType(const Handle<MemoryHeap>& memory_heap,
					   const VkMemoryType& vk_memory_type,
					   uint32 type_id)
	: Parent<MemoryHeap>(memory_heap)
	, VKMemoryType(vk_memory_type)
	, TypeID(type_id)
	, HeapID(vk_memory_type.heapIndex) {
	NotifyCreation(this);

	if (Parent<MemoryHeap>::Get().IsInvalid() || memory_heap->GetHeapID() != HeapID)
		throw std::runtime_error("MemoryType must be created with a valid MemoryHeap");
}

MemoryType::~MemoryType() {
	DestroyChildren();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */