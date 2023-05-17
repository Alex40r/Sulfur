#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IDescriptorSet.hpp"

class DescriptorSet : public IDescriptorSet, Parent<DescriptorLayout>, Parent<DescriptorPool> {
public:
	static Handle<DescriptorSet> Create(const Handle<DescriptorPool>& descriptor_pool,
										const Handle<DescriptorLayout>& descriptor_layout) {
		return new DescriptorSet(descriptor_pool, descriptor_layout);
	}

private:
	DescriptorSet(const Handle<DescriptorPool>& descriptor_pool,
				  const Handle<DescriptorLayout>& descriptor_layout);

public:
	~DescriptorSet() override;

	Handle<DescriptorSet> GetDescriptorSet() override { return this; }

	/* ---- ---- ---- ---- */

	VkDescriptorSet GetVKDescriptorSet() { return VKDescriptorSet; }

	/* ---- ---- ---- ---- */
private:
	VkDescriptorSet VKDescriptorSet;
};
