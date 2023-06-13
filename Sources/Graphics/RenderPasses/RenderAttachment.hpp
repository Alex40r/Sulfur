#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

enum AttachmentStoreOperation {
	ATTACHMENT_DONT_STORE = VK_ATTACHMENT_STORE_OP_DONT_CARE,
	ATTACHMENT_STORE = VK_ATTACHMENT_STORE_OP_STORE,
};

enum AttachmentLoadOperation {
	ATTACHMENT_DONT_LOAD = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
	ATTACHMENT_LOAD = VK_ATTACHMENT_LOAD_OP_LOAD,
	ATTACHMENT_CLEAR = VK_ATTACHMENT_LOAD_OP_CLEAR,
};

class RenderAttachment : public Object {
public:
	struct Descriptor {
		Descriptor() = default;
		Descriptor(const Handle<RenderAttachment>& render_attachment, VkImageLayout layout);

		bool IsValid() const { return Attachment.IsValid(); }
		bool IsInvalid() const { return Attachment.IsInvalid(); }

		operator Handle<RenderAttachment>() const { return Attachment; }

		Handle<RenderAttachment> Attachment;
		VkImageLayout Layout = VK_IMAGE_LAYOUT_UNDEFINED;
	};

public:
	static Handle<RenderAttachment> Create(const Handle<LogicalDevice>& logical_device,
										   VkFormat format,
										   VkImageLayout initial_layout,
										   VkImageLayout final_layout,
										   AttachmentLoadOperation color_depth_load_op,
										   AttachmentStoreOperation color_depth_store_op,
										   AttachmentLoadOperation stencil_load_op = ATTACHMENT_DONT_LOAD,
										   AttachmentStoreOperation stencil_store_op = ATTACHMENT_DONT_STORE,
										   VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT) {
		return new RenderAttachment(logical_device, format, initial_layout, final_layout, color_depth_load_op, color_depth_store_op, stencil_load_op, stencil_store_op, samples);
	}

private:
	RenderAttachment(const Handle<LogicalDevice>& logical_device,
					 VkFormat format,
					 VkImageLayout initial_layout,
					 VkImageLayout final_layout,
					 AttachmentLoadOperation color_depth_load_op,
					 AttachmentStoreOperation color_depth_store_op,
					 AttachmentLoadOperation stencil_load_op,
					 AttachmentStoreOperation stencil_store_op,
					 VkSampleCountFlagBits samples);

public:
	~RenderAttachment() override;

	/* ---- ---- ---- ---- */

	VkFormat GetFormat() const { return _Format; }
	VkSampleCountFlagBits GetSamples() const { return _Samples; }

	VkImageLayout GetInitialLayout() const { return _InitialLayout; }
	VkImageLayout GetFinalLayout() const { return _FinalLayout; }

	AttachmentLoadOperation GetColorDepthLoadOperation() const { return _ColorDepthLoadOperation; }
	AttachmentStoreOperation GetColorDepthStoreOperation() const { return _ColorDepthStoreOperation; }

	AttachmentLoadOperation GetStencilLoadOperation() const { return _StencilLoadOperation; }
	AttachmentStoreOperation GetStencilStoreOperation() const { return _StencilStoreOperation; }

    void GetVKAttachmentDescription(VkAttachmentDescription& vk_attachment_description) const;

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;

	/* ---- ---- ---- ---- */

	VkFormat _Format;
	VkSampleCountFlagBits _Samples;

	VkImageLayout _InitialLayout;
	VkImageLayout _FinalLayout;

	AttachmentLoadOperation _ColorDepthLoadOperation;
	AttachmentStoreOperation _ColorDepthStoreOperation;

	AttachmentLoadOperation _StencilLoadOperation;
	AttachmentStoreOperation _StencilStoreOperation;
};