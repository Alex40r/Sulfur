#include "DescriptorSet.hpp"

#include "DescriptorLayout.hpp"
#include "DescriptorPool.hpp"
#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Resources/Buffers/IBuffer.hpp"

DescriptorSet::DescriptorSet(const Handle<DescriptorPool>& descriptor_pool,
							 const Handle<DescriptorLayout>& descriptor_layout)
	: _DescriptorPool(descriptor_pool, this)
	, _DescriptorLayout(descriptor_layout, this) {
	NotifyCreation(this);

	if (_DescriptorPool.IsInvalid())
		throw std::runtime_error("DescriptorSet::DescriptorSet(): Descriptor pool is null");

	if (_DescriptorLayout.IsInvalid())
		throw std::runtime_error("DescriptorSet::DescriptorSet(): Descriptor layout is null");

	VkDescriptorSetAllocateInfo descriptor_set_allocation_info{};
	descriptor_set_allocation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptor_set_allocation_info.descriptorPool = _DescriptorPool->GetVKDescriptorPool();
	descriptor_set_allocation_info.descriptorSetCount = 1;
	VkDescriptorSetLayout vk_descriptor_layout = _DescriptorLayout->GetVKDescriptorLayout();
	descriptor_set_allocation_info.pSetLayouts = &vk_descriptor_layout;

	VkResult vk_result = vkAllocateDescriptorSets(_DescriptorPool->GetLogicalDevice()->GetVKLogicalDevice(), &descriptor_set_allocation_info, &_VKDescriptorSet);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("DescriptorSet::DescriptorSet(): Failed to allocate descriptor set");
}

DescriptorSet::~DescriptorSet() {
	Destroy();
	NotifyDestruction(this);

	vkFreeDescriptorSets(_DescriptorPool->GetLogicalDevice()->GetVKLogicalDevice(), _DescriptorPool->GetVKDescriptorPool(), 1, &_VKDescriptorSet);
}

/* ---- ---- ---- ---- */

void DescriptorSet::UpdateDescriptorBuffer(uint32 binding, uint32 element, const Handle<IBuffer>& buffer) {
	if (buffer.IsInvalid())
		throw std::runtime_error("DescriptorSet::UpdateDescriptor(): Buffer is null");

	VkDescriptorBufferInfo descriptor_buffer_info{};
	descriptor_buffer_info.buffer = buffer->GetVKBuffer();
	descriptor_buffer_info.offset = 0;
	descriptor_buffer_info.range = buffer->GetSize();

	VkWriteDescriptorSet write_info{};
	write_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write_info.dstSet = _VKDescriptorSet;
	write_info.dstBinding = binding;
	write_info.dstArrayElement = element;
	write_info.descriptorCount = 1;
	write_info.descriptorType = _DescriptorLayout->GetBinding(binding).Type;
	write_info.pBufferInfo = &descriptor_buffer_info;

	vkUpdateDescriptorSets(_DescriptorPool->GetLogicalDevice()->GetVKLogicalDevice(), 1, &write_info, 0, nullptr);
}

void DescriptorSet::UpdateDescriptor(uint32 binding, uint32 element, const Handle<ITexture>& texture) {
	if (texture.IsInvalid())
		throw std::runtime_error("DescriptorSet::UpdateDescriptor(): Texture is null");
}
