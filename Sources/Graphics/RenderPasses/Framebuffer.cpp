#include "Framebuffer.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Resources/Textures/TextureView.hpp"
#include "RenderPass.hpp"

Framebuffer::Framebuffer(const Handle<RenderPass>& render_pass,
						 const List<Handle<TextureView>>& texture_views,
						 uint32 width,
						 uint32 height,
						 uint32 layers)
	: _RenderPass(render_pass, this) {
	NotifyCreation(this);

	if (_RenderPass.IsInvalid())
		throw std::runtime_error("Framebuffer::Framebuffer(): Render pass is null");

	_TextureViews.Resize(texture_views.Length());
	for (uint32 i = 0; i < _TextureViews.Length(); i++)
		if (texture_views[i].IsValid())
			_TextureViews[i].Set(texture_views[i], this);
		else
			throw std::runtime_error("Framebuffer::Framebuffer(): Texture view is null");

	List<VkImageView> vk_image_views(_TextureViews.Length());
	for (uint32 i = 0; i < _TextureViews.Length(); i++)
		vk_image_views[i] = _TextureViews[i]->GetVKImageView();

	VkFramebufferCreateInfo vk_framebuffer_creation_info{};
	vk_framebuffer_creation_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	vk_framebuffer_creation_info.renderPass = _RenderPass->GetVKRenderPass();
	vk_framebuffer_creation_info.attachmentCount = _TextureViews.Length();
	vk_framebuffer_creation_info.pAttachments = vk_image_views.Data();
	vk_framebuffer_creation_info.width = width;
	vk_framebuffer_creation_info.height = height;
	vk_framebuffer_creation_info.layers = layers;

	VkResult vk_result = vkCreateFramebuffer(_RenderPass->GetLogicalDevice()->GetVKLogicalDevice(), &vk_framebuffer_creation_info, nullptr, &_VKFramebuffer);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Framebuffer::Framebuffer(): Failed to create framebuffer");
}

Framebuffer::~Framebuffer() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyFramebuffer(_RenderPass->GetLogicalDevice()->GetVKLogicalDevice(), _VKFramebuffer, nullptr);
}

/* ---- ---- ---- ---- */