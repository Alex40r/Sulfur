#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class RenderDependency : public Object {
public:
	static Handle<RenderDependency> Create(const Handle<LogicalDevice>& logical_device,
										   const Handle<RenderSubpass>& source_subpass,
										   const Handle<RenderSubpass>& distant_subpass,
										   VkPipelineStageFlags source_stage_mask,
										   VkPipelineStageFlags distant_stage_mask,
										   VkAccessFlags source_access_mask,
										   VkAccessFlags distant_access_mask,
										   VkDependencyFlags flags = 0) {
		return new RenderDependency(logical_device, source_subpass, distant_subpass, source_stage_mask, distant_stage_mask, source_access_mask, distant_access_mask, flags);
	}

private:
	RenderDependency(const Handle<LogicalDevice>& logical_device,
					 const Handle<RenderSubpass>& source_subpass,
					 const Handle<RenderSubpass>& distant_subpass,
					 VkPipelineStageFlags source_stage_mask,
					 VkPipelineStageFlags distant_stage_mask,
					 VkAccessFlags source_access_mask,
					 VkAccessFlags distant_access_mask,
					 VkDependencyFlags flags);

public:
	~RenderDependency() override;

	/* ---- ---- ---- ---- */

	VkPipelineStageFlags GetSourceStageMask() const { return _SourceStageMask; }
	VkPipelineStageFlags GetDistantStageMask() const { return _DistantStageMask; }

	VkAccessFlags GetSourceAccessMask() const { return _SourceAccessMask; }
	VkAccessFlags GetDistantAccessMask() const { return _DistantAccessMask; }

	VkDependencyFlags GetFlags() const { return _Flags; }

    void GetVKSubpassDependency(const Handle<RenderPass>& render_pass, VkSubpassDependency& dependency) const;

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<RenderSubpass>& GetSourceSubpass() const { return _SourceSubpass; }
	const ParentHandle<RenderSubpass>& GetDistantSubpass() const { return _DistantSubpass; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<RenderSubpass> _SourceSubpass;
	ParentHandle<RenderSubpass> _DistantSubpass;

	/* ---- ---- ---- ---- */

	VkPipelineStageFlags _SourceStageMask;
	VkPipelineStageFlags _DistantStageMask;

	VkAccessFlags _SourceAccessMask;
	VkAccessFlags _DistantAccessMask;

	VkDependencyFlags _Flags;
};