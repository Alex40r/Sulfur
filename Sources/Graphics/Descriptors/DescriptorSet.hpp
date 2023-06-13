#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class DescriptorSet : public Object {
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

	/* ---- ---- ---- ---- */

    VkDescriptorSet GetVKDescriptorSet() const { return _VKDescriptorSet; }

	void UpdateDescriptorBuffer(uint32 binding, uint32 element, const Handle<IBuffer>& buffer);
	void UpdateDescriptor(uint32 binding, uint32 element, const Handle<ITexture>& texture);

	/* ---- ---- ---- ---- */

	const ParentHandle<DescriptorPool>& GetDescriptorPool() const { return _DescriptorPool; }
	const ParentHandle<DescriptorLayout>& GetDescriptorLayout() const { return _DescriptorLayout; }

private:
	ParentHandle<DescriptorPool> _DescriptorPool;
	ParentHandle<DescriptorLayout> _DescriptorLayout;

	/* ---- ---- ---- ---- */

    VkDescriptorSet _VKDescriptorSet;
};