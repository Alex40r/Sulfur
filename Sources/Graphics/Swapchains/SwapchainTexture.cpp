#include "SwapchainTexture.hpp"

SwapchainTexture::SwapchainTexture(const Handle<Swapchain>& swapchain,
								   uint32 index,
								   VkImage vk_image)
	: _Swapchain(swapchain, this)
	, _VKImage(vk_image)
	, _SwapchainTextureID(index) {
	NotifyCreation(this);

	if (_Swapchain.IsInvalid())
		throw std::runtime_error("SwapchainTexture::SwapchainTexture(): Swapchain is null");

	if (_VKImage == VK_NULL_HANDLE)
		throw std::runtime_error("SwapchainTexture::SwapchainTexture(): Vulkan image is null");
}

SwapchainTexture::~SwapchainTexture() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void SwapchainTexture::Bind(const Handle<Memory>& memory, uint64 offset) {
	throw std::runtime_error("SwapchainTexture::Bind(): Cannot bind swapchain texture");
}