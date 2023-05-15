#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class DescriptorSet : public Object<LogicalDevice, DescriptorLayout, DescriptorPool> {
public:
	static Handle<DescriptorSet> Create(const Handle<LogicalDevice>& logical_device, const Handle<DescriptorLayout>& layout, const Handle<DescriptorPool>& pool) {
		return new DescriptorSet(logical_device, layout, pool);
	}

private:
	DescriptorSet(const Handle<LogicalDevice>& logical_device, const Handle<DescriptorLayout>& layout, const Handle<DescriptorPool>& pool);

public:
	~DescriptorSet();

	void UpdateDescriptor(uint32 binding, uint32 element, Buffer* buffer);

	VkDescriptorSet GetVKDescriptorSet() { return VKDescriptorSet; }

private:
	VkDescriptorSet VKDescriptorSet;
};
