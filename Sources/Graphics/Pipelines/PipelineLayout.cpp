#include "PipelineLayout.hpp"

#include "Graphics/Descriptors/DescriptorLayout.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"

PipelineLayout::PipelineLayout(const Handle<LogicalDevice>& logical_device,
							   const List<Handle<DescriptorLayout>>& descriptor_layouts)
	: _LogicalDevice(logical_device, this) {
	NotifyCreation(this);

    if(_LogicalDevice.IsInvalid())
        throw std::runtime_error("PipelineLayout::PipelineLayout(): Logical device is null");

    _DescriptorLayouts.Resize(descriptor_layouts.Length());
    for(uint32 i = 0; i < _DescriptorLayouts.Length(); i++)
        if(descriptor_layouts[i].IsValid())
            _DescriptorLayouts[i].Set(descriptor_layouts[i], this);
        else
            throw std::runtime_error("PipelineLayout::PipelineLayout(): Descriptor layout is null");

    List<VkDescriptorSetLayout> vk_descriptor_layouts(_DescriptorLayouts.Length());
    for(uint32 i = 0; i < _DescriptorLayouts.Length(); i++)
        vk_descriptor_layouts[i] = _DescriptorLayouts[i]->GetVKDescriptorLayout();

    VkPipelineLayoutCreateInfo vk_pipeline_layout_creation_info{};
    vk_pipeline_layout_creation_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vk_pipeline_layout_creation_info.setLayoutCount = _DescriptorLayouts.Length();
    vk_pipeline_layout_creation_info.pSetLayouts = vk_descriptor_layouts.Data();

    VkResult vk_result = vkCreatePipelineLayout(_LogicalDevice->GetVKLogicalDevice(), &vk_pipeline_layout_creation_info, nullptr, &_VKPipelineLayout);
    if(vk_result != VK_SUCCESS)
        throw std::runtime_error("PipelineLayout::PipelineLayout(): Failed to create pipeline layout");
}

PipelineLayout::~PipelineLayout() {
	Destroy();
	NotifyDestruction(this);

    vkDestroyPipelineLayout(_LogicalDevice->GetVKLogicalDevice(), _VKPipelineLayout, nullptr);
}

/* ---- ---- ---- ---- */