#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IMemoryType.hpp"

class MemoryType : public IMemoryType, Parent<MemoryHeap> {
	friend class PhysicalDevice;

private:
	static Handle<MemoryType> Create(const Handle<MemoryHeap>& memory_heap,
									 const VkMemoryType& vk_memory_type,
									 uint32 type_id) {
		return new MemoryType(memory_heap, vk_memory_type, type_id);
	}

private:
	MemoryType(const Handle<MemoryHeap>& memory_heap,
			   const VkMemoryType& vk_memory_type,
			   uint32 type_id);

public:
	~MemoryType() override;

	Handle<MemoryType> GetMemoryType() override { return this; }

	/* ---- ---- ---- ---- */

	uint32 GetTypeID() const { return TypeID; }
	uint32 GetHeapID() const { return HeapID; }
    
	/* ---- ---- ---- ---- */
private:
	VkMemoryType VKMemoryType;
	uint32 TypeID;
	uint32 HeapID;
};