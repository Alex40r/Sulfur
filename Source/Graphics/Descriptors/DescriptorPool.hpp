#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class DescriptorPool : public Object<LogicalDevice> {
public:
	struct Size {
		Size() = default;
		Size(VkDescriptorType type, uint32 count);

		VkDescriptorType DescriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		uint32 DescriptorCount = 0;

		void GetVKDescriptorPoolSize(VkDescriptorPoolSize& descriptor) const;
	};

	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(const List<DescriptorPool::Size>& sizes, uint32 max_set_count);

		List<DescriptorPool::Size> PoolSizes;
		uint32 MaximumSetCount = 0;
	};

	static Handle<DescriptorPool> Create(const Handle<LogicalDevice>& logical_device, const List<DescriptorPool::Size>& sizes, uint32 max_set_count) {
		return new DescriptorPool(logical_device, DescriptorPool::CreationInfo(sizes, max_set_count));
	}
	static Handle<DescriptorPool> Create(const Handle<LogicalDevice>& logical_device, const DescriptorPool::CreationInfo& creation_info) {
		return new DescriptorPool(logical_device, creation_info);
	}

private:
	DescriptorPool(const Handle<LogicalDevice>& logical_device, const DescriptorPool::CreationInfo& creation_info);

public:
	~DescriptorPool();

	const DescriptorPool::CreationInfo& GetCreationInfo() { return Info; }

	VkDescriptorPool GetVKDescriptorPool() { return VKDescriptorPool; }

private:
	DescriptorPool::CreationInfo Info;

	VkDescriptorPool VKDescriptorPool;
};