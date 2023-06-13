#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class DescriptorPool : public Object {
public:
	struct Size {
		Size() = default;
		Size(VkDescriptorType type, uint32 count);

		void GetVKDescriptorPoolSize(VkDescriptorPoolSize& descriptor) const;

		VkDescriptorType DescriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		uint32 DescriptorCount = 0;
	};

	/* ---- ---- ---- ---- */
public:
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

	/* ---- ---- ---- ---- */

	VkDescriptorPool GetVKDescriptorPool() const { return _VKDescriptorPool; }

	uint32 GetMaxSets() const { return _MaxSets; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;

	/* ---- ---- ---- ---- */

	VkDescriptorPool _VKDescriptorPool;

	List<Size> _Sizes;
	uint32 _MaxSets;
};