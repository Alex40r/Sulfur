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
	: _LogicalDevice(logical_device, this)
	, _Sizes(sizes)
	, _MaxSets(max_sets) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("DescriptorPool::DescriptorPool(): Logical device is null");

	List<VkDescriptorPoolSize> vk_sizes(_Sizes.Length());
	for (uint32 i = 0; i < _Sizes.Length(); ++i)
		_Sizes[i].GetVKDescriptorPoolSize(vk_sizes[i]);

	VkDescriptorPoolCreateInfo vk_descriptor_pool_creation_info{};
	vk_descriptor_pool_creation_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	vk_descriptor_pool_creation_info.poolSizeCount = vk_sizes.Length();
	vk_descriptor_pool_creation_info.pPoolSizes = vk_sizes.Data();
	vk_descriptor_pool_creation_info.maxSets = _MaxSets;
	vk_descriptor_pool_creation_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	VkResult vk_result = vkCreateDescriptorPool(_LogicalDevice->GetVKLogicalDevice(), &vk_descriptor_pool_creation_info, nullptr, &_VKDescriptorPool);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("DescriptorPool::DescriptorPool(): Failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyDescriptorPool(_LogicalDevice->GetVKLogicalDevice(), _VKDescriptorPool, nullptr);
}

/* ---- ---- ---- ---- */