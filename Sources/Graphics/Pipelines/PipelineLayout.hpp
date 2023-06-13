#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class PipelineLayout : public Object {
public:
	static Handle<PipelineLayout> Create(const Handle<LogicalDevice>& logical_device,
										 const List<Handle<DescriptorLayout>>& descriptor_layouts) {
		return new PipelineLayout(logical_device, descriptor_layouts);
	}

private:
	PipelineLayout(const Handle<LogicalDevice>& logical_device,
				   const List<Handle<DescriptorLayout>>& descriptor_layouts);

public:
	~PipelineLayout() override;

	/* ---- ---- ---- ---- */

    VkPipelineLayout GetVKPipelineLayout() const { return _VKPipelineLayout; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const List<ParentHandle<DescriptorLayout>>& GetDescriptorLayouts() const { return _DescriptorLayouts; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	List<ParentHandle<DescriptorLayout>> _DescriptorLayouts;

	/* ---- ---- ---- ---- */
    
    VkPipelineLayout _VKPipelineLayout;
};