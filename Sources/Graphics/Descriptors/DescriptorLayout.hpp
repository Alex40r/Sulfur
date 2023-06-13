#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class DescriptorLayout : public Object {
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

	/* ---- ---- ---- ---- */
public:
	static Handle<DescriptorLayout> Create(const Handle<LogicalDevice>& logical_device,
										   const List<Binding>& bindings) {
		return new DescriptorLayout(logical_device, bindings);
	}

private:
	DescriptorLayout(const Handle<LogicalDevice>& logical_device,
					 const List<Binding>& bindings);

public:
	~DescriptorLayout() override;

	/* ---- ---- ---- ---- */

	VkDescriptorSetLayout GetVKDescriptorLayout() const { return _VKDescriptorLayout; }

	const List<Binding>& GetBindings() const { return _Bindings; }
	const Binding& GetBinding(uint32 index) const { return _Bindings[index]; }
	uint32 GetBindingCount() const { return _Bindings.Length(); }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;

	/* ---- ---- ---- ---- */

	VkDescriptorSetLayout _VKDescriptorLayout;

	List<Binding> _Bindings;
};