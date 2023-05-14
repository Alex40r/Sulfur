#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

enum AttachmentStoreOperation {
	ATTACHMENT_DONT_STORE = VK_ATTACHMENT_STORE_OP_DONT_CARE,
	ATTACHMENT_STORE = VK_ATTACHMENT_STORE_OP_STORE,
};

enum AttachmentLoadOperation {
	ATTACHMENT_DONT_LOAD = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
	ATTACHMENT_LOAD = VK_ATTACHMENT_LOAD_OP_LOAD,
	ATTACHMENT_CLEAR = VK_ATTACHMENT_LOAD_OP_CLEAR,
};

class RenderAttachment : public Object<LogicalDevice> {
public:
	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(VkFormat format,
					 VkImageLayout initial_layout,
					 VkImageLayout final_layout,
					 AttachmentLoadOperation color_depth_load_op,
					 AttachmentStoreOperation color_depth_store_op,
					 AttachmentLoadOperation stencil_load_op,
					 AttachmentStoreOperation stencil_store_op,
					 VkSampleCountFlagBits samples);

		VkFormat Format;
		VkSampleCountFlagBits Samples;

		VkImageLayout InitialLayout;
		VkImageLayout FinalLayout;

		AttachmentLoadOperation ColorDepthLoadOperation;
		AttachmentStoreOperation ColorDepthStoreOperation;

		AttachmentLoadOperation StencilLoadOperation;
		AttachmentStoreOperation StencilStoreOperation;
	};

	static Handle<RenderAttachment> Create(const Handle<LogicalDevice>& logical_device,
										   VkFormat format,
										   VkImageLayout initial_layout,
										   VkImageLayout final_layout,
										   AttachmentLoadOperation color_depth_load_op,
										   AttachmentStoreOperation color_depth_store_op,
										   AttachmentLoadOperation stencil_load_op = ATTACHMENT_DONT_LOAD,
										   AttachmentStoreOperation stencil_store_op = ATTACHMENT_DONT_STORE,
										   VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT) {
		return new RenderAttachment(logical_device, RenderAttachment::CreationInfo(format, initial_layout, final_layout, color_depth_load_op, color_depth_store_op, stencil_load_op, stencil_store_op, samples));
	}
	static Handle<RenderAttachment> Create(const Handle<LogicalDevice>& logical_device,
										   const RenderAttachment::CreationInfo& creation_info) {
		return new RenderAttachment(logical_device, creation_info);
	}

private:
	RenderAttachment(const Handle<LogicalDevice>& logical_device,
					 const RenderAttachment::CreationInfo& creation_info);

public:
	~RenderAttachment();

	const RenderAttachment::CreationInfo& GetCreationInfo() { return Info; }

	void GetVKAttachmentDescription(VkAttachmentDescription& description);

private:
	RenderAttachment::CreationInfo Info;
};