#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class MemoryType : public Object<MemoryHeap> {
	friend class PhysicalDevice;

private:
	static Handle<MemoryType> Create(const Handle<MemoryHeap>& memory_heap,
									 const VkMemoryType& vk_memory_type,
									 uint32 type_id) {
		return new MemoryType(memory_heap, vk_memory_type, type_id);
	}

	MemoryType(const Handle<MemoryHeap>& memory_heap,
			   const VkMemoryType& vk_memory_type,
			   uint32 type_id);

public:
	~MemoryType();

	uint32 GetTypeID() const { return TypeID; }
	uint32 GetHeapID() const { return HeapID; }

private:
	VkMemoryType VKMemoryType;
	uint32 TypeID;
	uint32 HeapID;
};