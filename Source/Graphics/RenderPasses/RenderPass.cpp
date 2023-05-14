#include "RenderPass.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "RenderAttachment.hpp"
#include "RenderDependency.hpp"
#include "RenderSubpass.hpp"

RenderPass::RenderPass(const Handle<LogicalDevice>& logical_device,
					   const List<Handle<RenderAttachment>>& attachments,
					   const List<Handle<RenderSubpass>>& subpasses,
					   const List<Handle<RenderDependency>>& dependencies)
	: Object(logical_device, attachments, subpasses, dependencies) {
	VkResult vk_result;

	if (logical_device.IsInvalid())
		throw std::runtime_error("Invalid logical device");

	for (uint32 i = 0; i < attachments.GetLength(); i++)
		if (attachments[i].IsInvalid())
			throw std::runtime_error("Invalid attachment");

	for (uint32 i = 0; i < subpasses.GetLength(); i++)
		if (subpasses[i].IsInvalid())
			throw std::runtime_error("Invalid subpass");

	for (uint32 i = 0; i < dependencies.GetLength(); i++)
		if (dependencies[i].IsInvalid())
			throw std::runtime_error("Invalid dependency");

	NotifyCreation(this);

	List<VkAttachmentDescription> vk_attachment_descriptions(attachments.GetLength());
	for (uint32 i = 0; i < attachments.GetLength(); i++)
		attachments[i]->GetVKAttachmentDescription(vk_attachment_descriptions[i]);

	List<RenderSubpass::Description> subpass_descriptions(subpasses.GetLength());
	List<VkSubpassDescription> vk_subpass_descriptions(subpasses.GetLength());
	for (uint32 i = 0; i < subpasses.GetLength(); i++) {
		subpasses[i]->GetDescription(subpass_descriptions[i], this);
		subpass_descriptions[i].GetVKSubpassDescription(vk_subpass_descriptions[i]);
	}

	List<VkSubpassDependency> vk_subpass_dependencies(dependencies.GetLength());
	for (uint32 i = 0; i < dependencies.GetLength(); i++)
		dependencies[i]->GetVKSubpassDependency(vk_subpass_dependencies[i], this);

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

uint32 RenderPass::GetAttachmentID(const Handle<RenderAttachment>& attachment) {
	if (attachment.IsInvalid())
		return INVALID_ID;

	List<Handle<RenderAttachment>> attachments = Utils::GetParent<List<RenderAttachment>>(this);

	for (uint32 i = 0; i < attachments.GetLength(); i++)
		if (attachments[i] == attachment)
			return i;

	return INVALID_ID;
}

uint32 RenderPass::GetSubpassID(const Handle<RenderSubpass>& subpass) {
	if (subpass.IsInvalid())
		return INVALID_ID;

	List<Handle<RenderSubpass>> subpasses = Utils::GetParent<List<RenderSubpass>>(this);

	for (uint32 i = 0; i < subpasses.GetLength(); i++)
		if (subpasses[i] == subpass)
			return i;

	return INVALID_ID;
}

uint32 RenderPass::GetDependencyID(const Handle<RenderDependency>& dependency) {
	if (dependency.IsInvalid())
		return INVALID_ID;

	List<Handle<RenderDependency>> dependencies = Utils::GetParent<List<RenderDependency>>(this);

	for (uint32 i = 0; i < dependencies.GetLength(); i++)
		if (dependencies[i] == dependency)
			return i;

	return INVALID_ID;
}

RenderPass::~RenderPass() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroyRenderPass(Utils::GetParent<LogicalDevice>(this)->GetVKLogicalDevice(), VKRenderPass, nullptr);
}