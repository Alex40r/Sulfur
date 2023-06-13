#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class RenderPass : public Object {
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

	/* ---- ---- ---- ---- */

	VkRenderPass GetVKRenderPass() const { return _VKRenderPass; }

	uint32 GetAttachmentID(const Handle<RenderAttachment>& attachment) const;
	uint32 GetSubpassID(const Handle<RenderSubpass>& subpass) const;
	uint32 GetDependencyID(const Handle<RenderDependency>& dependency) const;

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const List<ParentHandle<RenderAttachment>>& GetAttachments() const { return _Attachments; }
	const List<ParentHandle<RenderSubpass>>& GetSubpasses() const { return _Subpasses; }
	const List<ParentHandle<RenderDependency>>& GetDependencies() const { return _Dependencies; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	List<ParentHandle<RenderAttachment>> _Attachments;
	List<ParentHandle<RenderSubpass>> _Subpasses;
	List<ParentHandle<RenderDependency>> _Dependencies;

	/* ---- ---- ---- ---- */

	VkRenderPass _VKRenderPass;

	/* ---- ---- ---- ---- */
private:
	struct SubpassDescriptor {
		SubpassDescriptor() = default;
		static void Create(const Handle<RenderPass>& render_pass, const Handle<RenderSubpass>& subpass, SubpassDescriptor& subpass_descriptor);

		List<VkAttachmentReference> InputAttachments;
		List<VkAttachmentReference> ColorAttachments;
		List<VkAttachmentReference> ResolveAttachments;
		List<uint32> PreserveAttachments;

		VkAttachmentReference DepthStencilAttachment = {};

		void GetVKSubpassDescription(VkSubpassDescription& vk_description) const;
	};
};