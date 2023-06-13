#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IBuffer.hpp"

class Buffer : public IBuffer {
public:
	static Handle<Buffer> Create(const Handle<LogicalDevice>& logical_device,
								 uint64 size,
								 VkBufferUsageFlags usage,
								 VkBufferCreateFlags flags = 0) {
		return new Buffer(logical_device, size, usage, flags);
	}

private:
	Buffer(const Handle<LogicalDevice>& logical_device,
		   uint64 size,
		   VkBufferUsageFlags usage,
		   VkBufferCreateFlags flags);

public:
	~Buffer() override;

	/* ---- ---- ---- ---- */

	VkBuffer GetVKBuffer() const override { return _VKBuffer; }
	uint64 GetSize() const override { return _Size; }

	void Bind(const Handle<Memory>& memory, uint64 offset = 0) override;
	bool IsBound() const override { return _BindMemory.IsValid(); }
	IResource::Requirements GetRequirements() const override;
	uint64 GetBindOffset() const { return _BindOffset; }


	/* ---- ---- ---- ---- */

	const Handle<Memory>& GetBoundMemory() { return _BindMemory; }
	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<Memory> _BindMemory;

	/* ---- ---- ---- ---- */

	uint64 _BindOffset;

	VkBuffer _VKBuffer;
	VkMemoryRequirements _VKMemoryRequirements;

	uint64 _Size;

	VkBufferUsageFlags _Usage;
	VkBufferCreateFlags _Flags;
};