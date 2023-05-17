#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IDescriptorPool.hpp"

class DescriptorPool : public IDescriptorPool, Parent<LogicalDevice> {

public:
	struct Size {
		Size() = default;
		Size(VkDescriptorType type, uint32 count);

		void GetVKDescriptorPoolSize(VkDescriptorPoolSize& descriptor) const;

		VkDescriptorType DescriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		uint32 DescriptorCount = 0;
	};

	static Handle<DescriptorPool> Create(const Handle<LogicalDevice>& logical_device,
										 const List<Size>& sizes,
										 uint32 max_sets) {
		return new DescriptorPool(logical_device, sizes, max_sets);
	}

private:
	DescriptorPool(const Handle<LogicalDevice>& logical_device,
				   const List<Size>& sizes,
				   uint32 max_sets);

public:
	~DescriptorPool() override;

	Handle<DescriptorPool> GetDescriptorPool() override { return this; }

	/* ---- ---- ---- ---- */

	const Handle<LogicalDevice>& GetLogicalDevice() { return Parent<LogicalDevice>::Get(); }
	VkDescriptorPool GetVKDescriptorPool() { return VKDescriptorPool; }

	/* ---- ---- ---- ---- */
private:

	VkDescriptorPool VKDescriptorPool;
};