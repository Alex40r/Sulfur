#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class RenderPass : public Object<LogicalDevice, List<RenderAttachment>, List<RenderSubpass>, List<RenderDependency>> {
public:
	static Handle<RenderPass> Create(const Handle<LogicalDevice>& logical_device,
									 const List<Handle<RenderAttachment>>& attachments,
									 const List<Handle<RenderSubpass>>& subpasses,
									 const List<Handle<RenderDependency>>& dependencies) {
		return new RenderPass(logical_device, attachments, subpasses, dependencies);
	}

private:
	RenderPass(const Handle<LogicalDevice>& logical_device,
			   const List<Handle<RenderAttachment>>& attachments,
			   const List<Handle<RenderSubpass>>& subpasses,
			   const List<Handle<RenderDependency>>& dependencies);

public:
	~RenderPass();

	uint32 GetAttachmentID(const Handle<RenderAttachment>& attachment);
	uint32 GetSubpassID(const Handle<RenderSubpass>& subpass);
	uint32 GetDependencyID(const Handle<RenderDependency>& dependency);

private:
	VkRenderPass VKRenderPass;
};