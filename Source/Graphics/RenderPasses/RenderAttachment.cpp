#include "RenderAttachment.hpp"

RenderAttachment::CreationInfo::CreationInfo(VkFormat format, VkImageLayout initial_layout, VkImageLayout final_layout, AttachmentLoadOperation color_depth_load_op, AttachmentStoreOperation color_depth_store_op, AttachmentLoadOperation stencil_load_op, AttachmentStoreOperation stencil_store_op, VkSampleCountFlagBits samples)
	: Format(format)
	, Samples(samples)
	, InitialLayout(initial_layout)
	, FinalLayout(final_layout)
	, ColorDepthLoadOperation(color_depth_load_op)
	, ColorDepthStoreOperation(color_depth_store_op)
	, StencilLoadOperation(stencil_load_op)
	, StencilStoreOperation(stencil_store_op) {
}

RenderAttachment::RenderAttachment(const Handle<LogicalDevice>& logical_device, const RenderAttachment::CreationInfo& creation_info)
	: Object(logical_device)
	, Info(creation_info) {
	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	NotifyCreation(this);
}

RenderAttachment::~RenderAttachment() {
	DestroyChildren();
	NotifyDestruction(this);
}

void RenderAttachment::GetVKAttachmentDescription(VkAttachmentDescription& description) {
	description.format = Info.Format;
	description.samples = Info.Samples;
	description.loadOp = static_cast<VkAttachmentLoadOp>(Info.ColorDepthLoadOperation);
	description.storeOp = static_cast<VkAttachmentStoreOp>(Info.ColorDepthStoreOperation);
	description.stencilLoadOp = static_cast<VkAttachmentLoadOp>(Info.StencilLoadOperation);
	description.stencilStoreOp = static_cast<VkAttachmentStoreOp>(Info.StencilStoreOperation);
	description.initialLayout = Info.InitialLayout;
	description.finalLayout = Info.FinalLayout;
	description.flags = 0;
}