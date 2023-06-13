#pragma once

#include "Utils/Utils.hpp"

#include "Graphics/Resources/Textures/ITexture.hpp"

class SwapchainTexture : public ITexture {
	friend class Swapchain;

private:
	SwapchainTexture(const Handle<Swapchain>& swapchain,
					 uint32 index,
					 VkImage vk_image);

public:
	~SwapchainTexture() override;

	/* ---- ---- ---- ---- */

	VkImage GetVKImage() const override { return _VKImage; }

	bool IsBound() const override { return true; }
	void Bind(const Handle<Memory>& memory, uint64 offset) override;
	IResource::Requirements GetRequirements() const override { return IResource::Requirements(); }

	uint32 GetSwapchainTextureID() const { return _SwapchainTextureID; }

	/* ---- ---- ---- ---- */

	const ParentHandle<Swapchain>& GetSwapchain() const { return _Swapchain; }

private:
	ParentHandle<Swapchain> _Swapchain;

	/* ---- ---- ---- ---- */

	uint32 _SwapchainTextureID;

	VkImage _VKImage;
};