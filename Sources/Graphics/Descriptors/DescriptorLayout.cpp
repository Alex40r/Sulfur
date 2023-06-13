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
	vk_binding.descriptorType = Type;
	vk_binding.descriptorCount = DescriptorCount;
	vk_binding.stageFlags = Stages;
}

/* ---- ---- ---- ---- */

#include "DescriptorLayout.hpp"

DescriptorLayout::DescriptorLayout(const Handle<LogicalDevice>& logical_device,
								   const List<Binding>& bindings)
	: _LogicalDevice(logical_device, this)
	, _Bindings(bindings) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("DescriptorLayout::DescriptorLayout(): Logical device is null");

	List<VkDescriptorSetLayoutBinding> vk_bindings(_Bindings.Length());
	for (uint32 i = 0; i < _Bindings.Length(); ++i)
		_Bindings[i].GetVKDescriptorLayoutBinding(vk_bindings[i]);

	VkDescriptorSetLayoutCreateInfo vk_descriptor_layout_creation_info{};
	vk_descriptor_layout_creation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	vk_descriptor_layout_creation_info.bindingCount = vk_bindings.Length();
	vk_descriptor_layout_creation_info.pBindings = vk_bindings.Data();

	VkResult vk_result = vkCreateDescriptorSetLayout(_LogicalDevice->GetVKLogicalDevice(), &vk_descriptor_layout_creation_info, nullptr, &_VKDescriptorLayout);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("DescriptorLayout::DescriptorLayout(): Failed to create descriptor layout");
}

DescriptorLayout::~DescriptorLayout() {
	Destroy();
	NotifyDestruction(this);

    vkDestroyDescriptorSetLayout(_LogicalDevice->GetVKLogicalDevice(), _VKDescriptorLayout, nullptr);
}

/* ---- ---- ---- ---- */