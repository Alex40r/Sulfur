#include "DescriptorSet.hpp"

#include "DescriptorLayout.hpp"
#include "DescriptorPool.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"

DescriptorSet::DescriptorSet(const Handle<DescriptorPool>& descriptor_pool,
							 const Handle<DescriptorLayout>& descriptor_layout)
	: Parent<DescriptorPool>(descriptor_pool)
	, Parent<DescriptorLayout>(descriptor_layout) {
	NotifyCreation(this);

	if (Parent<DescriptorPool>::Get().IsInvalid())
		throw std::runtime_error("DescriptorSet must be created with a valid DescriptorPool");

	if (Parent<DescriptorLayout>::Get().IsInvalid())
		throw std::runtime_error("DescriptorSet must be created with a valid DescriptorLayout");

	VkResult vk_result;

	VkDescriptorSetAllocateInfo allocation_info{};
	allocation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocation_info.descriptorPool = descriptor_pool->GetVKDescriptorPool();
	allocation_info.descriptorSetCount = 1;
	VkDescriptorSetLayout vk_layout = descriptor_layout->GetVKDescriptorLayout();
	allocation_info.pSetLayouts = &vk_layout;

	vk_result = vkAllocateDescriptorSets(descriptor_pool->GetLogicalDevice()->GetVKLogicalDevice(), &allocation_info, &VKDescriptorSet);

	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate descriptor set");
}

DescriptorSet::~DescriptorSet() {
	DestroyChildren();
	NotifyDestruction(this);

	vkFreeDescriptorSets(Parent<DescriptorPool>::Get()->GetLogicalDevice()->GetVKLogicalDevice(), Parent<DescriptorPool>::Get()->GetVKDescriptorPool(), 1, &VKDescriptorSet);
}

/* ---- ---- ---- ---- */
