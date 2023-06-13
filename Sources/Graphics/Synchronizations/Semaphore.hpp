#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class Semaphore : public Object {
public:
	static Handle<Semaphore> Create(const Handle<LogicalDevice>& logical_device) {
		return new Semaphore(logical_device);
	}

private:
	Semaphore(const Handle<LogicalDevice>& logical_device);

public:
	~Semaphore() override;

	/* ---- ---- ---- ---- */

	VkSemaphore GetVKSemaphore() const { return _VKSemaphore; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;

	/* ---- ---- ---- ---- */

	VkSemaphore _VKSemaphore;
};