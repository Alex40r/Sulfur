#include "RenderDependency.hpp"

#include "RenderPass.hpp"
#include "RenderSubpass.hpp"

RenderDependency::RenderDependency(const Handle<LogicalDevice>& logical_device,
								   const Handle<RenderSubpass>& source_subpass,
								   const Handle<RenderSubpass>& distant_subpass,
								   VkPipelineStageFlags source_stage_mask,
								   VkPipelineStageFlags distant_stage_mask,
								   VkAccessFlags source_access_mask,
								   VkAccessFlags distant_access_mask,
								   VkDependencyFlags flags)
	: Parent<LogicalDevice>(logical_device)
	, Parent<List<RenderSubpass>>({source_subpass, distant_subpass})
	, SourceSubpass(source_subpass)
	, DistantSubpass(distant_subpass)
	, SourceStageMask(source_stage_mask)
	, DistantStageMask(distant_stage_mask)
	, SourceAccessMask(source_access_mask)
	, DistantAccessMask(distant_access_mask)
	, Flags(flags) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("RenderDependency must be created with a valid LogicalDevice");
}

RenderDependency::~RenderDependency() {
	DestroyChildren();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void RenderDependency::GetVKSubpassDependency(const Handle<RenderPass>& render_pass, VkSubpassDependency& vk_subpass_dependency) {
	if (SourceSubpass.IsInvalid())
		vk_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	else
		vk_subpass_dependency.srcSubpass = render_pass->GetSubpassID(SourceSubpass);

	if (DistantSubpass.IsInvalid())
		vk_subpass_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	else
		vk_subpass_dependency.dstSubpass = render_pass->GetSubpassID(DistantSubpass);

	vk_subpass_dependency.srcStageMask = SourceStageMask;
	vk_subpass_dependency.dstStageMask = DistantStageMask;

	vk_subpass_dependency.srcAccessMask = SourceAccessMask;
	vk_subpass_dependency.dstAccessMask = DistantAccessMask;

	vk_subpass_dependency.dependencyFlags = Flags;
}
