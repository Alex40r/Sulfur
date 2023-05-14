#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

struct RenderAttachmentDescriptor {
	RenderAttachmentDescriptor() = default;
	RenderAttachmentDescriptor(const Handle<RenderAttachment>& render_attachment, VkImageLayout layout);

	Handle<RenderAttachment> Attachment;
	VkImageLayout Layout = VK_IMAGE_LAYOUT_UNDEFINED;

	bool IsValid() const { return Attachment.IsValid(); }
	bool IsInvalid() const { return Attachment.IsInvalid(); }

	static List<Handle<RenderAttachment>> GetAttachments(const List<RenderAttachmentDescriptor>& descriptors);
	static List<Handle<RenderAttachment>> GetAttachments(const RenderAttachmentDescriptor& descriptor);
};

class RenderSubpass : public Object<LogicalDevice, List<RenderAttachment>> {
public:
	struct Description {
		Description() = default;

		List<VkAttachmentReference> InputAttachments;
		List<VkAttachmentReference> ColorAttachments;
		List<VkAttachmentReference> ResolveAttachments;

		VkAttachmentReference DepthStencilAttachment = {};

		List<uint32> PreserveAttachments;

		void GetVKSubpassDescription(VkSubpassDescription& vk_description);
	};

	static Handle<RenderSubpass> Create(const Handle<LogicalDevice>& logical_device,
										const List<RenderAttachmentDescriptor>& input_attachments,
										const List<RenderAttachmentDescriptor>& color_attachments,
										const List<RenderAttachmentDescriptor>& resolve_attachments,
										const RenderAttachmentDescriptor& depth_stencil_attachment,
										const List<Handle<RenderAttachment>>& preserve_attachments) {
		return new RenderSubpass(logical_device, input_attachments, color_attachments, resolve_attachments, depth_stencil_attachment, preserve_attachments);
	}

private:
	RenderSubpass(const Handle<LogicalDevice>& logical_device,
				  const List<RenderAttachmentDescriptor>& input_attachments,
				  const List<RenderAttachmentDescriptor>& color_attachments,
				  const List<RenderAttachmentDescriptor>& resolve_attachments,
				  const RenderAttachmentDescriptor& depth_stencil_attachment,
				  const List<Handle<RenderAttachment>>& preserve_attachments);

public:
	~RenderSubpass();

	void GetDescription(RenderSubpass::Description& description, const Handle<RenderPass>& render_pass);

private:
	List<RenderAttachmentDescriptor> InputAttachments;
	List<RenderAttachmentDescriptor> ColorAttachments;
	List<RenderAttachmentDescriptor> ResolveAttachments;

	RenderAttachmentDescriptor DepthStencilAttachment;

	List<Handle<RenderAttachment>> PreserveAttachments;
};