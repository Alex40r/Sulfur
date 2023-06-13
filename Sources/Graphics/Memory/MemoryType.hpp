#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class MemoryType : public Object {
	friend class PhysicalDevice;

private:
	MemoryType(const Handle<MemoryHeap>& memory_heap,
			   uint32 index,
			   VkMemoryType vk_memory_type);

public:
	~MemoryType() override;

	/* ---- ---- ---- ---- */

	uint32 GetTypeID() const { return _MemoryTypeID; }
	uint32 GetHeapID() const { return _VKMemoryType.heapIndex; }

	VkMemoryPropertyFlags GetPropertyFlags() const { return _VKMemoryType.propertyFlags; }

	/* ---- ---- ---- ---- */

	const ParentHandle<MemoryHeap>& GetMemoryHeap() const { return _MemoryHeap; }

private:
	ParentHandle<MemoryHeap> _MemoryHeap;

	/* ---- ---- ---- ---- */

	uint32 _MemoryTypeID;

	VkMemoryType _VKMemoryType;
};