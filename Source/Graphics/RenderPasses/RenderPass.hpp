#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IRenderPass.hpp"

class RenderPass : public IRenderPass, Parent<LogicalDevice>, Parent<List<RenderAttachment>>, Parent<List<RenderSubpass>>, Parent<List<RenderDependency>> {
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
	~RenderPass() override;

	Handle<RenderPass> GetRenderPass() override { return this; }

	/* ---- ---- ---- ---- */

	uint32 GetAttachmentID(const Handle<RenderAttachment>& attachment);
	uint32 GetSubpassID(const Handle<RenderSubpass>& subpass);

	/* ---- ---- ---- ---- */
private:
	VkRenderPass VKRenderPass;
};