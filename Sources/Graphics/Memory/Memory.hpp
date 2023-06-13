#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class Memory : public Object {
public:
	static Handle<Memory> Create(const Handle<LogicalDevice>& logical_device,
								 const Handle<MemoryType>& memory_type,
								 uint64 size) {
		return new Memory(logical_device, memory_type, size);
	}
	static Handle<Memory> Create(const Handle<LogicalDevice>& logical_device,
								 const List<Handle<IResource>>& resources,
								 AllocationType allocation_type = PREFER_DEVICE_LOCAL,
								 VkMemoryPropertyFlags target_properties = 0) {
		return new Memory(logical_device, resources, allocation_type, target_properties);
	}

private:
	Memory(const Handle<LogicalDevice>& logical_device,
		   const Handle<MemoryType>& memory_type,
		   uint64 size);
	Memory(const Handle<LogicalDevice>& logical_device,
		   const List<Handle<IResource>>& resources,
		   AllocationType allocation_type,
		   VkMemoryPropertyFlags target_properties);

public:
	~Memory() override;

	/* ---- ---- ---- ---- */

	VkDeviceMemory GetVKDeviceMemory() const { return _VKDeviceMemory; }

	uint64 GetSize() const { return _Size; }

	void* MapMemory(uint64 size, uint64 offset);
	void UnmapMemory();

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<MemoryType>& GetMemoryType() const { return _MemoryType; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<MemoryType> _MemoryType;

	/* ---- ---- ---- ---- */

	VkDeviceMemory _VKDeviceMemory;

	uint64 _Size;
};