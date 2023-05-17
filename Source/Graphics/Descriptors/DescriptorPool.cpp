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

/* ---- ---- ---- ---- */

DescriptorPool::DescriptorPool(const Handle<LogicalDevice>& logical_device,
							   const List<Size>& sizes,
							   uint32 max_sets)
	: Parent<LogicalDevice>(logical_device) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("DescriptorPool must be created with a valid LogicalDevice");

	VkResult vk_result;

	List<VkDescriptorPoolSize> vk_sizes(sizes.GetLength());
	for (uint32 i = 0; i < sizes.GetLength(); i++)
		sizes[i].GetVKDescriptorPoolSize(vk_sizes[i]);


	VkDescriptorPoolCreateInfo pool_creation_info{};
	pool_creation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_creation_info.poolSizeCount = vk_sizes.GetLength();
	pool_creation_info.pPoolSizes = vk_sizes.GetPointer();
	pool_creation_info.maxSets = max_sets;

	vk_result = vkCreateDescriptorPool(logical_device->GetVKLogicalDevice(), &pool_creation_info, nullptr, &VKDescriptorPool);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyDescriptorPool(Parent<LogicalDevice>::Get()->GetVKLogicalDevice(), VKDescriptorPool, nullptr);
}

/* ---- ---- ---- ---- */
