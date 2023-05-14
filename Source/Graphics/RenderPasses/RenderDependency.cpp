#include "RenderDependency.hpp"

#include "Graphics/RenderPasses/RenderPass.hpp"

RenderDependency::CreationInfo::CreationInfo(VkPipelineStageFlags source_stage_mask, VkPipelineStageFlags distant_stage_mask, VkAccessFlags source_access_mask, VkAccessFlags distant_access_mask, VkDependencyFlags flags)
	: SourceStageMask(source_stage_mask)
	, DistantStageMask(distant_stage_mask)
	, SourceAccessMask(source_access_mask)
	, DistantAccessMask(distant_access_mask)
	, Flags(flags) {
}

RenderDependency::RenderDependency(const Handle<LogicalDevice>& logical_device, const Handle<RenderSubpass>& source_subpass, const Handle<RenderSubpass>& distant_subpass, const RenderDependency::CreationInfo& creation_info)
	: Object(logical_device, {source_subpass, distant_subpass})
	, Info(creation_info)
	, SourceSubpass(source_subpass)
	, DistantSubpass(distant_subpass) {
	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	NotifyCreation(this);
}

RenderDependency::~RenderDependency() {
	DestroyChildren();
	NotifyDestruction(this);

}

void RenderDependency::GetVKSubpassDependency(VkSubpassDependency& vk_subpass_dependency, const Handle<RenderPass>& render_pass) {
	if (SourceSubpass.IsInvalid())
		vk_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	else
		vk_subpass_dependency.srcSubpass = render_pass->GetSubpassID(SourceSubpass);

	if (DistantSubpass.IsInvalid())
		vk_subpass_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	else
		vk_subpass_dependency.dstSubpass = render_pass->GetSubpassID(DistantSubpass);

	vk_subpass_dependency.srcStageMask = Info.SourceStageMask;
	vk_subpass_dependency.dstStageMask = Info.DistantStageMask;

	vk_subpass_dependency.srcAccessMask = Info.SourceAccessMask;
	vk_subpass_dependency.dstAccessMask = Info.DistantAccessMask;

	vk_subpass_dependency.dependencyFlags = Info.Flags;
}
