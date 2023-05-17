#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IRenderDependency.hpp"

class RenderDependency : public IRenderDependency, Parent<LogicalDevice>, Parent<List<RenderSubpass>> {
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

	Handle<RenderDependency> GetRenderDependency() override { return this; }

	/* ---- ---- ---- ---- */

	void GetVKSubpassDependency(const Handle<RenderPass>& render_pass, VkSubpassDependency& vk_subpass_dependency);

	/* ---- ---- ---- ---- */
private:
	Handle<RenderSubpass> SourceSubpass;
	Handle<RenderSubpass> DistantSubpass;

	VkPipelineStageFlags SourceStageMask;
	VkPipelineStageFlags DistantStageMask;

	VkAccessFlags SourceAccessMask;
	VkAccessFlags DistantAccessMask;

	VkDependencyFlags Flags;
};