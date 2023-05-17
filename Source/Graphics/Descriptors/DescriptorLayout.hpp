#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IDescriptorLayout.hpp"

class DescriptorLayout : public IDescriptorLayout, Parent<LogicalDevice> {
public:
	struct Binding {
		Binding() = default;
		Binding(VkDescriptorType type, uint32 binding, uint32 descriptor_count, VkShaderStageFlags stages);
		
		void GetVKDescriptorLayoutBinding(VkDescriptorSetLayoutBinding& vk_binding) const;


		VkDescriptorType Type = VK_DESCRIPTOR_TYPE_SAMPLER;
		VkShaderStageFlags Stages = 0;
		uint32 BindingID = 0;
		uint32 DescriptorCount = 0;
	};

	static Handle<DescriptorLayout> Create(const Handle<LogicalDevice>& logical_device,
										   const List<Binding>& bindings) {
		return new DescriptorLayout(logical_device, bindings);
	}

private:
	DescriptorLayout(const Handle<LogicalDevice>& logical_device,
					 const List<Binding>& bindings);

public:
	~DescriptorLayout() override;

	Handle<DescriptorLayout> GetDescriptorLayout() override { return this; }

	/* ---- ---- ---- ---- */

	VkDescriptorSetLayout GetVKDescriptorLayout() { return VKDescriptorLayout; }

	/* ---- ---- ---- ---- */
private:
	VkDescriptorSetLayout VKDescriptorLayout;
	List<Binding> Bindings;
};
