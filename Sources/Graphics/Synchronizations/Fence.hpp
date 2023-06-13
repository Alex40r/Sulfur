#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class Fence : public Object {
public:
	static Handle<Fence> Create(const Handle<LogicalDevice>& logical_device,
								bool signaled = true) {
		return new Fence(logical_device, signaled);
	}

private:
	Fence(const Handle<LogicalDevice>& logical_device,
		  bool signaled);

public:
	~Fence() override;

	/* ---- ---- ---- ---- */

	VkFence GetVKFence() const { return _VKFence; }

	void Wait(uint64 timeout = 0xFFFFFFFFFFFFFFFF);
	void Reset();

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;

	/* ---- ---- ---- ---- */

	VkFence _VKFence;
};