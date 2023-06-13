#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class Framebuffer : public Object {
public:
	static Handle<Framebuffer> Create(const Handle<RenderPass>& render_pass,
									  const List<Handle<TextureView>>& texture_views,
									  uint32 width,
									  uint32 height,
									  uint32 layers = 1) {
		return new Framebuffer(render_pass, texture_views, width, height, layers);
	}

private:
	Framebuffer(const Handle<RenderPass>& render_pass,
				const List<Handle<TextureView>>& texture_views,
				uint32 width,
				uint32 height,
				uint32 layers);

public:
	~Framebuffer() override;

	/* ---- ---- ---- ---- */

	VkFramebuffer GetVKFramebuffer() const { return _VKFramebuffer; }

	/* ---- ---- ---- ---- */

	const ParentHandle<RenderPass>& GetRenderPass() const { return _RenderPass; }
	const List<ParentHandle<TextureView>>& GetTextureViews() const { return _TextureViews; }

private:
	ParentHandle<RenderPass> _RenderPass;
	List<ParentHandle<TextureView>> _TextureViews;

	/* ---- ---- ---- ---- */

	VkFramebuffer _VKFramebuffer;
};