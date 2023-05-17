#include "RenderAttachment.hpp"

RenderAttachment::RenderAttachment(const Handle<LogicalDevice>& logical_device,
								   VkFormat format,
								   VkImageLayout initial_layout,
								   VkImageLayout final_layout,
								   AttachmentLoadOperation color_depth_load_op,
								   AttachmentStoreOperation color_depth_store_op,
								   AttachmentLoadOperation stencil_load_op,
								   AttachmentStoreOperation stencil_store_op,
								   VkSampleCountFlagBits samples)
	: Parent<LogicalDevice>(logical_device)
	, Format(format)
	, Samples(samples)
	, InitialLayout(initial_layout)
	, FinalLayout(final_layout)
	, ColorDepthLoadOperation(color_depth_load_op)
	, ColorDepthStoreOperation(color_depth_store_op)
	, StencilLoadOperation(stencil_load_op)
	, StencilStoreOperation(stencil_store_op) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("RenderAttachment must be created with a valid LogicalDevice");
}

RenderAttachment::~RenderAttachment() {
	DestroyChildren();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void RenderAttachment::GetVKAttachmentDescription(VkAttachmentDescription& description) {
	description.format = Format;
	description.samples = Samples;
	description.loadOp = static_cast<VkAttachmentLoadOp>(ColorDepthLoadOperation);
	description.storeOp = static_cast<VkAttachmentStoreOp>(ColorDepthStoreOperation);
	description.stencilLoadOp = static_cast<VkAttachmentLoadOp>(StencilLoadOperation);
	description.stencilStoreOp = static_cast<VkAttachmentStoreOp>(StencilStoreOperation);
	description.initialLayout = InitialLayout;
	description.finalLayout = FinalLayout;
	description.flags = 0;
}
