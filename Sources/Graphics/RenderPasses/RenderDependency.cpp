#include "RenderDependency.hpp"

#include "RenderPass.hpp"

RenderDependency::RenderDependency(const Handle<LogicalDevice>& logical_device,
								   const Handle<RenderSubpass>& source_subpass,
								   const Handle<RenderSubpass>& distant_subpass,
								   VkPipelineStageFlags source_stage_mask,
								   VkPipelineStageFlags distant_stage_mask,
								   VkAccessFlags source_access_mask,
								   VkAccessFlags distant_access_mask,
								   VkDependencyFlags flags)
	: _LogicalDevice(logical_device, this)
	, _SourceSubpass(source_subpass, this)
	, _DistantSubpass(distant_subpass, this)
	, _SourceStageMask(source_stage_mask)
	, _DistantStageMask(distant_stage_mask)
	, _SourceAccessMask(source_access_mask)
	, _DistantAccessMask(distant_access_mask)
	, _Flags(flags) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("RenderDependency::RenderDependency(): Logical device is null");
}

RenderDependency::~RenderDependency() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void RenderDependency::GetVKSubpassDependency(const Handle<RenderPass>& render_pass, VkSubpassDependency& dependency) const {
	if (_SourceSubpass.IsInvalid())
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	else {
		dependency.srcSubpass = render_pass->GetSubpassID(_SourceSubpass);
		if (dependency.srcSubpass == INVALID_ID)
			throw std::runtime_error("RenderDependency::GetVKSubpassDependency(): Source subpass is not part of the render pass");
	}

	if (_DistantSubpass.IsInvalid())
		dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	else {
		dependency.dstSubpass = render_pass->GetSubpassID(_DistantSubpass);
		if (dependency.dstSubpass == INVALID_ID)
			throw std::runtime_error("RenderDependency::GetVKSubpassDependency(): Distant subpass is not part of the render pass");
	}

	dependency.srcStageMask = _SourceStageMask;
	dependency.dstStageMask = _DistantStageMask;

	dependency.srcAccessMask = _SourceAccessMask;
	dependency.dstAccessMask = _DistantAccessMask;

	dependency.dependencyFlags = _Flags;
}
