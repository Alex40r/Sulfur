#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class DescriptorLayout : public Object<LogicalDevice> {
public:
	struct Binding {
		Binding() = default;
		Binding(VkDescriptorType type, uint32 binding, uint32 descriptor_count, VkShaderStageFlags stages);

		VkDescriptorType Type = VK_DESCRIPTOR_TYPE_SAMPLER;
		VkShaderStageFlags Stages = 0;
		uint32 BindingID = 0;
		uint32 DescriptorCount = 0;

		void GetVKDescriptorLayoutBinding(VkDescriptorSetLayoutBinding& vk_binding) const;
	};

	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(const List<Binding>& bindings);

		List<Binding> Bindings;
	};

	static Handle<DescriptorLayout> Create(const Handle<LogicalDevice>& logical_device, const List<Binding>& bindings) {
		return new DescriptorLayout(logical_device, DescriptorLayout::CreationInfo(bindings));
	}
	static Handle<DescriptorLayout> Create(const Handle<LogicalDevice>& logical_device, const DescriptorLayout::CreationInfo& creation_info) {
		return new DescriptorLayout(logical_device, creation_info);
	}

private:
	DescriptorLayout(const Handle<LogicalDevice>& logical_device, const DescriptorLayout::CreationInfo& creation_info);

public:
	~DescriptorLayout();

	const DescriptorLayout::CreationInfo& GetCreationInfo() { return Info; }

private:
	DescriptorLayout::CreationInfo Info;

	VkDescriptorSetLayout VKDescriptorLayout;
	List<VkDescriptorSetLayoutBinding> VKBindings;
};
