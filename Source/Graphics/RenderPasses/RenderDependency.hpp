#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class RenderDependency : public Object<LogicalDevice, List<RenderSubpass>> {
public:
	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(VkPipelineStageFlags source_stage_mask,
					 VkPipelineStageFlags distant_stage_mask,
					 VkAccessFlags source_access_mask,
					 VkAccessFlags distant_access_mask,
					 VkDependencyFlags flags);

		VkPipelineStageFlags SourceStageMask = VK_PIPELINE_STAGE_NONE;
		VkPipelineStageFlags DistantStageMask = VK_PIPELINE_STAGE_NONE;

		VkAccessFlags SourceAccessMask = VK_ACCESS_NONE;
		VkAccessFlags DistantAccessMask = VK_ACCESS_NONE;

		VkDependencyFlags Flags = 0;
	};

	static Handle<RenderDependency> Create(const Handle<LogicalDevice>& logical_device,
										   const Handle<RenderSubpass>& source_subpass,
										   const Handle<RenderSubpass>& distant_subpass,
										   VkPipelineStageFlags source_stage_mask,
										   VkPipelineStageFlags distant_stage_mask,
										   VkAccessFlags source_access_mask,
										   VkAccessFlags distant_access_mask,
										   VkDependencyFlags flags = 0) {
		return new RenderDependency(logical_device, source_subpass, distant_subpass, RenderDependency::CreationInfo(source_stage_mask, distant_stage_mask, source_access_mask, distant_access_mask, flags));
	}
	static Handle<RenderDependency> Create(const Handle<LogicalDevice>& logical_device,
										   const Handle<RenderSubpass>& source_subpass,
										   const Handle<RenderSubpass>& distant_subpass,
										   const RenderDependency::CreationInfo& creation_info) {
		return new RenderDependency(logical_device, source_subpass, distant_subpass, creation_info);
	}

private:
	RenderDependency(const Handle<LogicalDevice>& logical_device,
					 const Handle<RenderSubpass>& source_subpass,
					 const Handle<RenderSubpass>& distant_subpass,
					 const RenderDependency::CreationInfo& creation_info);

public:
	~RenderDependency();

	const RenderDependency::CreationInfo& GetCreationInfo() { return Info; }

	void GetVKSubpassDependency(VkSubpassDependency& vk_subpass_dependency, const Handle<RenderPass>& render_pass);

private:
	RenderDependency::CreationInfo Info;

	Handle<RenderSubpass> SourceSubpass;
	Handle<RenderSubpass> DistantSubpass;
};