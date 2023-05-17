#include "DescriptorLayout.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"

DescriptorLayout::Binding::Binding(VkDescriptorType type, uint32 binding, uint32 descriptor_count, VkShaderStageFlags stages)
	: Type(type)
	, Stages(stages)
	, BindingID(binding)
	, DescriptorCount(descriptor_count) {
}

void DescriptorLayout::Binding::GetVKDescriptorLayoutBinding(VkDescriptorSetLayoutBinding& vk_binding) const {
	vk_binding.binding = BindingID;
	vk_binding.descriptorCount = DescriptorCount;
	vk_binding.descriptorType = Type;
	vk_binding.stageFlags = Stages;
}

/* ---- ---- ---- ---- */

DescriptorLayout::DescriptorLayout(const Handle<LogicalDevice>& logical_device,
								   const List<Binding>& bindings)
	: Parent<LogicalDevice>(logical_device) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("DescriptorLayout must be created with a valid LogicalDevice");

	VkResult vk_result;

	List<VkDescriptorSetLayoutBinding> vk_bindings(bindings.GetLength());
	for (uint32 i = 0; i < bindings.GetLength(); ++i)
		bindings[i].GetVKDescriptorLayoutBinding(vk_bindings[i]);

	VkDescriptorSetLayoutCreateInfo layout_creation_info{};
	layout_creation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_creation_info.bindingCount = vk_bindings.GetLength();
	layout_creation_info.pBindings = vk_bindings.GetPointer();

	vk_result = vkCreateDescriptorSetLayout(logical_device->GetVKLogicalDevice(), &layout_creation_info, nullptr, &VKDescriptorLayout);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor layout");
}

DescriptorLayout::~DescriptorLayout() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyDescriptorSetLayout(Parent<LogicalDevice>::Get()->GetVKLogicalDevice(), VKDescriptorLayout, nullptr);
}

/* ---- ---- ---- ---- */
