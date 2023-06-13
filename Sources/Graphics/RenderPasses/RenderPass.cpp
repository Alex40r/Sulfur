#include "RenderPass.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "RenderAttachment.hpp"
#include "RenderDependency.hpp"
#include "RenderSubpass.hpp"

RenderPass::RenderPass(const Handle<LogicalDevice>& logical_device,
					   const List<Handle<RenderAttachment>>& attachments,
					   const List<Handle<RenderSubpass>>& subpasses,
					   const List<Handle<RenderDependency>>& dependencies)
	: _LogicalDevice(logical_device, this) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("RenderPass::RenderPass(): Logical device is null");

	_Attachments.Resize(attachments.Length());
	for (uint32 i = 0; i < _Attachments.Length(); i++)
		if (attachments[i].IsValid())
			_Attachments[i].Set(attachments[i], this);
		else
			throw std::runtime_error("RenderPass::RenderPass(): Attachment is null");

	_Subpasses.Resize(subpasses.Length());
	for (uint32 i = 0; i < _Subpasses.Length(); i++)
		if (subpasses[i].IsValid())
			_Subpasses[i].Set(subpasses[i], this);
		else
			throw std::runtime_error("RenderPass::RenderPass(): Subpass is null");

	_Dependencies.Resize(dependencies.Length());
	for (uint32 i = 0; i < _Dependencies.Length(); i++)
		if (dependencies[i].IsValid())
			_Dependencies[i].Set(dependencies[i], this);
		else
			throw std::runtime_error("RenderPass::RenderPass(): Dependency is null");

	/* ---- ---- ---- ---- */
	List<VkAttachmentDescription> vk_attachments(_Attachments.Length());
	for (uint32 i = 0; i < _Attachments.Length(); i++)
		_Attachments[i]->GetVKAttachmentDescription(vk_attachments[i]);

	/* ---- ---- ---- ---- */
	List<SubpassDescriptor> subpass_desriptors(_Subpasses.Length());
	List<VkSubpassDescription> vk_subpasses(_Subpasses.Length());
	for (uint32 i = 0; i < _Subpasses.Length(); i++) {
		SubpassDescriptor::Create(this, _Subpasses[i], subpass_desriptors[i]);
		subpass_desriptors[i].GetVKSubpassDescription(vk_subpasses[i]);
	}

	/* ---- ---- ---- ---- */
	List<VkSubpassDependency> vk_dependencies(_Dependencies.Length());
	for (uint32 i = 0; i < _Dependencies.Length(); i++)
		_Dependencies[i]->GetVKSubpassDependency(this, vk_dependencies[i]);

	/* ---- ---- ---- ---- */
	VkRenderPassCreateInfo vk_render_pass_creation_info{};

	vk_render_pass_creation_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	vk_render_pass_creation_info.attachmentCount = vk_attachments.Length();
	vk_render_pass_creation_info.pAttachments = vk_attachments.Data();

	vk_render_pass_creation_info.subpassCount = vk_subpasses.Length();
	vk_render_pass_creation_info.pSubpasses = vk_subpasses.Data();

	vk_render_pass_creation_info.dependencyCount = vk_dependencies.Length();
	vk_render_pass_creation_info.pDependencies = vk_dependencies.Data();

	if (vkCreateRenderPass(_LogicalDevice->GetVKLogicalDevice(), &vk_render_pass_creation_info, nullptr, &_VKRenderPass) != VK_SUCCESS)
		throw std::runtime_error("RenderPass::RenderPass(): Failed to create render pass");
}

RenderPass::~RenderPass() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyRenderPass(_LogicalDevice->GetVKLogicalDevice(), _VKRenderPass, nullptr);
}

/* ---- ---- ---- ---- */

uint32 RenderPass::GetAttachmentID(const Handle<RenderAttachment>& attachment) const {
	if (attachment.IsInvalid())
		return INVALID_ID;

	for (uint32 i = 0; i < _Attachments.Length(); i++)
		if (_Attachments[i] == attachment)
			return i;

	return INVALID_ID;
}

uint32 RenderPass::GetSubpassID(const Handle<RenderSubpass>& subpass) const {
	if (subpass.IsInvalid())
		return INVALID_ID;

	for (uint32 i = 0; i < _Subpasses.Length(); i++)
		if (_Subpasses[i] == subpass)
			return i;

	return INVALID_ID;
}

uint32 RenderPass::GetDependencyID(const Handle<RenderDependency>& dependency) const {
	if (dependency.IsInvalid())
		return INVALID_ID;

	for (uint32 i = 0; i < _Dependencies.Length(); i++)
		if (_Dependencies[i] == dependency)
			return i;

	return INVALID_ID;
}

/* ---- ---- ---- ---- */

void RenderPass::SubpassDescriptor::Create(const Handle<RenderPass>& render_pass, const Handle<RenderSubpass>& subpass, SubpassDescriptor& subpass_descriptor) {
	subpass_descriptor.InputAttachments.Resize(subpass->GetInputAttachments().Length());
	for (uint32 i = 0; i < subpass_descriptor.InputAttachments.Length(); i++) {
		subpass_descriptor.InputAttachments[i].attachment = render_pass->GetAttachmentID(subpass->GetInputAttachments()[i]);
		subpass_descriptor.InputAttachments[i].layout = subpass->GetInputAttachments()[i].Layout;

		if (subpass_descriptor.InputAttachments[i].attachment == INVALID_ID)
			throw std::runtime_error("RenderPass::SubpassDescriptor::Create(): Input attachment is not part of the render pass");
	}

	/* ---- ---- ---- ---- */
	subpass_descriptor.ColorAttachments.Resize(subpass->GetColorAttachments().Length());
	for (uint32 i = 0; i < subpass_descriptor.ColorAttachments.Length(); i++) {
		subpass_descriptor.ColorAttachments[i].attachment = render_pass->GetAttachmentID(subpass->GetColorAttachments()[i]);
		subpass_descriptor.ColorAttachments[i].layout = subpass->GetColorAttachments()[i].Layout;

		if (subpass_descriptor.ColorAttachments[i].attachment == INVALID_ID)
			throw std::runtime_error("RenderPass::SubpassDescriptor::Create(): Color attachment is not part of the render pass");
	}

	/* ---- ---- ---- ---- */
	subpass_descriptor.ResolveAttachments.Resize(subpass->GetResolveAttachments().Length());
	for (uint32 i = 0; i < subpass_descriptor.ResolveAttachments.Length(); i++) {
		subpass_descriptor.ResolveAttachments[i].attachment = render_pass->GetAttachmentID(subpass->GetResolveAttachments()[i]);
		subpass_descriptor.ResolveAttachments[i].layout = subpass->GetResolveAttachments()[i].Layout;

		if (subpass_descriptor.ResolveAttachments[i].attachment == INVALID_ID)
			throw std::runtime_error("RenderPass::SubpassDescriptor::Create(): Resolve attachment is not part of the render pass");
	}

	/* ---- ---- ---- ---- */
	subpass_descriptor.PreserveAttachments.Resize(subpass->GetPreserveAttachments().Length());
	for (uint32 i = 0; i < subpass_descriptor.PreserveAttachments.Length(); i++) {
		subpass_descriptor.PreserveAttachments[i] = render_pass->GetAttachmentID(subpass->GetPreserveAttachments()[i]);

		if (subpass_descriptor.PreserveAttachments[i] == INVALID_ID)
			throw std::runtime_error("RenderPass::SubpassDescriptor::Create(): Preserve attachment is not part of the render pass");
	}

	/* ---- ---- ---- ---- */
	subpass_descriptor.DepthStencilAttachment.attachment = INVALID_ID;
	subpass_descriptor.DepthStencilAttachment.layout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (subpass->GetDepthStencilAttachment().IsValid()) {
		subpass_descriptor.DepthStencilAttachment.attachment = render_pass->GetAttachmentID(subpass->GetDepthStencilAttachment());
		subpass_descriptor.DepthStencilAttachment.layout = subpass->GetDepthStencilAttachment().Layout;

		if (subpass_descriptor.DepthStencilAttachment.attachment == INVALID_ID)
			throw std::runtime_error("RenderPass::SubpassDescriptor::Create(): Depth Stencil attachment is not part of the render pass");
	}
}

void RenderPass::SubpassDescriptor::GetVKSubpassDescription(VkSubpassDescription& vk_description) const {
	vk_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	vk_description.inputAttachmentCount = InputAttachments.Length();
	vk_description.pInputAttachments = InputAttachments.Data();

	vk_description.colorAttachmentCount = ColorAttachments.Length();
	vk_description.pColorAttachments = ColorAttachments.Data();

	vk_description.pResolveAttachments = ResolveAttachments.Data();

	vk_description.pDepthStencilAttachment = DepthStencilAttachment.attachment == INVALID_ID ? nullptr : &DepthStencilAttachment;

	vk_description.preserveAttachmentCount = PreserveAttachments.Length();
	vk_description.pPreserveAttachments = PreserveAttachments.Data();
}
