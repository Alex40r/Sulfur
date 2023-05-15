#include "DescriptorSet.hpp"

#include "DescriptorLayout.hpp"
#include "DescriptorPool.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"

DescriptorSet::DescriptorSet(const Handle<LogicalDevice>& logical_device, const Handle<DescriptorLayout>& layout, const Handle<DescriptorPool>& pool)
	: Object(logical_device, layout, pool) {
	VkResult vk_result;

	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	if (layout.IsInvalid())
		throw std::runtime_error("Invalid descriptor layout");

	NotifyCreation(this);

	VkDescriptorSetAllocateInfo allocation_info{};
	allocation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocation_info.descriptorPool = pool->GetVKDescriptorPool();
	allocation_info.descriptorSetCount = 1;
	VkDescriptorSetLayout vk_layout = layout->GetVKDescriptorLayout();
	allocation_info.pSetLayouts = &vk_layout;

	vk_result = vkAllocateDescriptorSets(logical_device->GetVKLogicalDevice(), &allocation_info, &VKDescriptorSet);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate descriptor set");
}

DescriptorSet::~DescriptorSet() {
	DestroyChildren();
	NotifyDestruction(this);

	vkFreeDescriptorSets(Utils::GetParent<LogicalDevice>(this)->GetVKLogicalDevice(), Utils::GetParent<DescriptorPool>(this)->GetVKDescriptorPool(), 1, &VKDescriptorSet);
}