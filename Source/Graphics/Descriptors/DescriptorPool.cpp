#include "DescriptorPool.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"

DescriptorPool::Size::Size(VkDescriptorType type, uint32 count)
	: DescriptorType(type)
	, DescriptorCount(count) {
}

void DescriptorPool::Size::GetVKDescriptorPoolSize(VkDescriptorPoolSize& descriptor) const {
	descriptor.type = DescriptorType;
	descriptor.descriptorCount = DescriptorCount;
}

DescriptorPool::CreationInfo::CreationInfo(const List<DescriptorPool::Size>& sizes, uint32 max_set_count)
	: PoolSizes(sizes)
	, MaximumSetCount(max_set_count) {
}

DescriptorPool::DescriptorPool(const Handle<LogicalDevice>& logical_device, const DescriptorPool::CreationInfo& creation_info)
	: Object(logical_device)
	, Info(creation_info) {
	VkResult vk_result;

	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	NotifyCreation(this);

	List<VkDescriptorPoolSize> vk_sizes(creation_info.PoolSizes.GetLength());

	for (uint32 i = 0; i < creation_info.PoolSizes.GetLength(); i++)
		creation_info.PoolSizes[i].GetVKDescriptorPoolSize(vk_sizes[i]);

	VkDescriptorPoolCreateInfo pool_creation_info{};
	pool_creation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_creation_info.poolSizeCount = vk_sizes.GetLength();
	pool_creation_info.pPoolSizes = vk_sizes.GetPointer();
	pool_creation_info.maxSets = creation_info.MaximumSetCount;

	vk_result = vkCreateDescriptorPool(logical_device->GetVKLogicalDevice(), &pool_creation_info, nullptr, &VKDescriptorPool);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyDescriptorPool(Utils::GetParent<LogicalDevice>(this)->GetVKLogicalDevice(), VKDescriptorPool, nullptr);
}