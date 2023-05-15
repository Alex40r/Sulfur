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

DescriptorLayout::CreationInfo::CreationInfo(const List<Binding>& bindings)
	: Bindings(bindings) {
}

DescriptorLayout::DescriptorLayout(const Handle<LogicalDevice>& logical_device, const DescriptorLayout::CreationInfo& creation_info)
	: Object(logical_device)
	, Info(creation_info) {
	VkResult vk_result;

	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	NotifyCreation(this);

	VKBindings.Resize(creation_info.Bindings.GetLength());

	for (uint32 i = 0; i < creation_info.Bindings.GetLength(); i++)
		creation_info.Bindings[i].GetVKDescriptorLayoutBinding(VKBindings[i]);

	VkDescriptorSetLayoutCreateInfo layout_creation_info{};
	layout_creation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_creation_info.bindingCount = VKBindings.GetLength();
	layout_creation_info.pBindings = VKBindings.GetPointer();

	vk_result = vkCreateDescriptorSetLayout(logical_device->GetVKLogicalDevice(), &layout_creation_info, nullptr, &VKDescriptorLayout);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Could not create descriptor layout");
}

DescriptorLayout::~DescriptorLayout() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyDescriptorSetLayout(Utils::GetParent<LogicalDevice>(this)->GetVKLogicalDevice(), VKDescriptorLayout, nullptr);
}
