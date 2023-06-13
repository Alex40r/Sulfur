#include "RenderAttachment.hpp"

RenderAttachment::Descriptor::Descriptor(const Handle<RenderAttachment>& render_attachment, VkImageLayout layout)
	: Attachment(render_attachment)
	, Layout(layout) {
}

/* ---- ---- ---- ---- */

RenderAttachment::RenderAttachment(const Handle<LogicalDevice>& logical_device,
								   VkFormat format,
								   VkImageLayout initial_layout,
								   VkImageLayout final_layout,
								   AttachmentLoadOperation color_depth_load_op,
								   AttachmentStoreOperation color_depth_store_op,
								   AttachmentLoadOperation stencil_load_op,
								   AttachmentStoreOperation stencil_store_op,
								   VkSampleCountFlagBits samples)
	: _LogicalDevice(logical_device, this)
	, _Format(format)
	, _Samples(samples)
	, _InitialLayout(initial_layout)
	, _FinalLayout(final_layout)
	, _ColorDepthLoadOperation(color_depth_load_op)
	, _ColorDepthStoreOperation(color_depth_store_op)
	, _StencilLoadOperation(stencil_load_op)
	, _StencilStoreOperation(stencil_store_op) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("RenderAttachment::RenderAttachment(): Logical device is null");
}

RenderAttachment::~RenderAttachment() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void RenderAttachment::GetVKAttachmentDescription(VkAttachmentDescription& vk_attachment_description) const {
	vk_attachment_description = {};

	vk_attachment_description.format = _Format;
	vk_attachment_description.samples = _Samples;
	vk_attachment_description.loadOp = static_cast<VkAttachmentLoadOp>(_ColorDepthLoadOperation);
	vk_attachment_description.storeOp = static_cast<VkAttachmentStoreOp>(_ColorDepthStoreOperation);
	vk_attachment_description.stencilLoadOp = static_cast<VkAttachmentLoadOp>(_StencilLoadOperation);
	vk_attachment_description.stencilStoreOp = static_cast<VkAttachmentStoreOp>(_StencilStoreOperation);
	vk_attachment_description.initialLayout = _InitialLayout;
	vk_attachment_description.finalLayout = _FinalLayout;
}
