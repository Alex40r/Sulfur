#include "RenderPass.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "RenderAttachment.hpp"
#include "RenderSubpass.hpp"
#include "RenderDependency.hpp"

RenderPass::RenderPass(const Handle<LogicalDevice>& logical_device,
					   const List<Handle<RenderAttachment>>& attachments,
					   const List<Handle<RenderSubpass>>& subpasses,
					   const List<Handle<RenderDependency>>& dependencies)
	: Parent<LogicalDevice>(logical_device)
	, Parent<List<RenderAttachment>>(attachments)
	, Parent<List<RenderSubpass>>(subpasses)
	, Parent<List<RenderDependency>>(dependencies) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("RenderPass must be created with a valid LogicalDevice");

	for (uint32 i = 0; i < attachments.GetLength(); i++)
		if (attachments[i].IsInvalid())
			throw std::runtime_error("RenderPass must be created with valid RenderAttachments");

	for (uint32 i = 0; i < subpasses.GetLength(); i++)
		if (subpasses[i].IsInvalid())
			throw std::runtime_error("RenderPass must be created with valid RenderSubpasses");

	for (uint32 i = 0; i < dependencies.GetLength(); i++)
		if (dependencies[i].IsInvalid())
			throw std::runtime_error("RenderPass must be created with valid RenderDependencies");

	VkResult vk_result;

	List<VkAttachmentDescription> vk_attachment_descriptions(attachments.GetLength());
	for (uint32 i = 0; i < attachments.GetLength(); i++)
		attachments[i]->GetVKAttachmentDescription(vk_attachment_descriptions[i]);

	List<RenderSubpass::Description> subpass_descriptions(subpasses.GetLength());
	List<VkSubpassDescription> vk_subpass_descriptions(subpasses.GetLength());
	for (uint32 i = 0; i < subpasses.GetLength(); i++) {
		subpasses[i]->GetDescription(this, subpass_descriptions[i]);
		subpass_descriptions[i].GetVKSubpassDescription(vk_subpass_descriptions[i]);
	}

	List<VkSubpassDependency> vk_subpass_dependencies(dependencies.GetLength());
	for (uint32 i = 0; i < dependencies.GetLength(); i++)
		dependencies[i]->GetVKSubpassDependency(this, vk_subpass_dependencies[i]);

	VkRenderPassCreateInfo render_pass_creation_info{};
	render_pass_creation_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	render_pass_creation_info.attachmentCount = vk_attachment_descriptions.GetLength();
	render_pass_creation_info.pAttachments = vk_attachment_descriptions.GetPointer();

	render_pass_creation_info.subpassCount = vk_subpass_descriptions.GetLength();
	render_pass_creation_info.pSubpasses = vk_subpass_descriptions.GetPointer();

    render_pass_creation_info.dependencyCount = vk_subpass_dependencies.GetLength();
    render_pass_creation_info.pDependencies = vk_subpass_dependencies.GetPointer();

	vk_result = vkCreateRenderPass(logical_device->GetVKLogicalDevice(), &render_pass_creation_info, nullptr, &VKRenderPass);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed create render pass");
}

RenderPass::~RenderPass() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyRenderPass(Parent<LogicalDevice>::Get()->GetVKLogicalDevice(), VKRenderPass, nullptr);
}

/* ---- ---- ---- ---- */

uint32 RenderPass::GetAttachmentID(const Handle<RenderAttachment>& attachment) {
	const List<Handle<RenderAttachment>>& attachments = Parent<List<RenderAttachment>>::Get();

	for (uint32 i = 0; i < attachments.GetLength(); ++i)
		if (attachments[i] == attachment)
			return i;

	return INVALID_ID;
}

uint32 RenderPass::GetSubpassID(const Handle<RenderSubpass>& subpass) {
	const List<Handle<RenderSubpass>>& subpasses = Parent<List<RenderSubpass>>::Get();

	for (uint32 i = 0; i < subpasses.GetLength(); ++i)
		if (subpasses[i] == subpass)
			return i;

	return INVALID_ID;
}
