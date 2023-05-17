#include "RenderSubpass.hpp"

#include "RenderAttachment.hpp"
#include "RenderPass.hpp"

RenderAttachmentDescriptor::RenderAttachmentDescriptor(const Handle<RenderAttachment>& render_attachment, VkImageLayout layout)
	: Attachment(render_attachment)
	, Layout(layout) {
}

List<Handle<RenderAttachment>> RenderAttachmentDescriptor::GetAttachments(const List<RenderAttachmentDescriptor>& descriptors) {
	List<Handle<RenderAttachment>> attachments(descriptors.GetLength());

	for (uint32 i = 0; i < descriptors.GetLength(); i++)
		attachments[i] = descriptors[i].Attachment;

	return attachments;
}

List<Handle<RenderAttachment>> RenderAttachmentDescriptor::GetAttachments(const RenderAttachmentDescriptor& descriptor) {
	if (descriptor.IsInvalid())
		return {};

	return {descriptor.Attachment};
}

/* ---- ---- ---- ---- */

void RenderSubpass::Description::GetVKSubpassDescription(VkSubpassDescription& vk_description) const {
	vk_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	vk_description.inputAttachmentCount = InputAttachments.GetLength();
	vk_description.pInputAttachments = InputAttachments.GetPointer();

	vk_description.colorAttachmentCount = ColorAttachments.GetLength();
	vk_description.pColorAttachments = ColorAttachments.GetPointer();

	vk_description.pResolveAttachments = ResolveAttachments.GetPointer();

	vk_description.pDepthStencilAttachment = &DepthStencilAttachment;

	vk_description.preserveAttachmentCount = PreserveAttachments.GetLength();
	vk_description.pPreserveAttachments = PreserveAttachments.GetPointer();
}

/* ---- ---- ---- ---- */

RenderSubpass::RenderSubpass(const Handle<LogicalDevice>& logical_device,
							 const List<RenderAttachmentDescriptor>& input_attachments,
							 const List<RenderAttachmentDescriptor>& color_attachments,
							 const List<RenderAttachmentDescriptor>& resolve_attachments,
							 const RenderAttachmentDescriptor& depth_stencil_attachment,
							 const List<Handle<RenderAttachment>>& preserve_attachments)
	: Parent<LogicalDevice>(logical_device)
	, Parent<List<RenderAttachment>>({RenderAttachmentDescriptor::GetAttachments(input_attachments),
									  RenderAttachmentDescriptor::GetAttachments(color_attachments),
									  RenderAttachmentDescriptor::GetAttachments(resolve_attachments),
									  RenderAttachmentDescriptor::GetAttachments(depth_stencil_attachment),
									  preserve_attachments})
	, InputAttachments(input_attachments)
	, ColorAttachments(color_attachments)
	, ResolveAttachments(resolve_attachments)
	, DepthStencilAttachment(depth_stencil_attachment)
	, PreserveAttachments(preserve_attachments) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("RenderSubpass must be created with a valid LogicalDevice");

	for (uint32 i = 0; i < input_attachments.GetLength(); i++)
		if (input_attachments[i].IsInvalid())
			throw std::runtime_error("RenderSubpass must be created with valid input RenderAttachments");

	for (uint32 i = 0; i < color_attachments.GetLength(); i++)
		if (color_attachments[i].IsInvalid())
			throw std::runtime_error("RenderSubpass must be created with valid color RenderAttachments");

	for (uint32 i = 0; i < resolve_attachments.GetLength(); i++)
		if (resolve_attachments[i].IsInvalid())
			throw std::runtime_error("RenderSubpass must be created with valid resolve RenderAttachments");

	for (uint32 i = 0; i < preserve_attachments.GetLength(); i++)
		if (preserve_attachments[i].IsInvalid())
			throw std::runtime_error("RenderSubpass must be created with valid preserve RenderAttachments");
}

RenderSubpass::~RenderSubpass() {
	DestroyChildren();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void RenderSubpass::GetDescription(const Handle<RenderPass>& render_pass, RenderSubpass::Description& description) const {
	description.InputAttachments.Resize(InputAttachments.GetLength());
	for (uint32 i = 0; i < InputAttachments.GetLength(); i++) {
		description.InputAttachments[i].attachment = render_pass->GetAttachmentID(InputAttachments[i].Attachment);
		description.InputAttachments[i].layout = InputAttachments[i].Layout;
	}

	description.ColorAttachments.Resize(ColorAttachments.GetLength());
	for (uint32 i = 0; i < ColorAttachments.GetLength(); i++) {
		description.ColorAttachments[i].attachment = render_pass->GetAttachmentID(ColorAttachments[i].Attachment);
		description.ColorAttachments[i].layout = ColorAttachments[i].Layout;
	}

	description.ResolveAttachments.Resize(ResolveAttachments.GetLength());
	for (uint32 i = 0; i < ResolveAttachments.GetLength(); i++) {
		description.ResolveAttachments[i].attachment = render_pass->GetAttachmentID(ResolveAttachments[i].Attachment);
		description.ResolveAttachments[i].layout = ResolveAttachments[i].Layout;
	}

	description.DepthStencilAttachment.attachment = render_pass->GetAttachmentID(DepthStencilAttachment.Attachment);
	description.DepthStencilAttachment.layout = DepthStencilAttachment.Layout;

	description.PreserveAttachments.Resize(PreserveAttachments.GetLength());
	for (uint32 i = 0; i < PreserveAttachments.GetLength(); i++)
		description.PreserveAttachments[i] = render_pass->GetAttachmentID(PreserveAttachments[i]);
}
