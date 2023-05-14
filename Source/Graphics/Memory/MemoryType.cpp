#include "MemoryType.hpp"

#include "MemoryHeap.hpp"

MemoryType::MemoryType(const Handle<MemoryHeap>& memory_heap, const VkMemoryType& vk_memory_type, uint32 type_id)
	: Object(memory_heap)
	, VKMemoryType(vk_memory_type)
	, TypeID(type_id)
	, HeapID(vk_memory_type.heapIndex) {
	if (memory_heap.IsInvalid())
		throw std::runtime_error("Invalid memory heap");

	if (memory_heap->GetHeapID() != HeapID)
		throw std::runtime_error("Memory heap mismatch");

	NotifyCreation(this);
}

MemoryType::~MemoryType() {
	DestroyChildren();
	NotifyDestruction(this);
}