#include "Swapchain.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Synchronizations/Fence.hpp"
#include "Graphics/Synchronizations/Semaphore.hpp"
#include "SwapchainTexture.hpp"
#include "WindowSurface.hpp"

Swapchain::Swapchain(const Handle<LogicalDevice>& logical_device,
					 const Handle<WindowSurface>& window_surface,
					 VkFormat format,
					 VkColorSpaceKHR color_space,
					 VkPresentModeKHR present_mode,
					 const Handle<Swapchain>& old_swapchain,
					 uint32 frame_count)
	: _LogicalDevice(logical_device, this)
	, _WindowSurface(window_surface, this)
	, _PresentMode(present_mode)
	, _FrameCount(frame_count)
	, _Capabilities(window_surface, logical_device->GetPhysicalDevice()) {
	NotifyCreation(this);

	if (_LogicalDevice == nullptr)
		throw std::runtime_error("Swapchain::Swapchain(): Logical device is null");

	if (_WindowSurface == nullptr)
		throw std::runtime_error("Swapchain::Swapchain(): Window surface is null");

	_SurfaceFormat.format = format;
	_SurfaceFormat.colorSpace = color_space;

	if (!_Capabilities.IsFormatSupported(_SurfaceFormat))
		throw std::runtime_error("Swapchain::Swapchain(): Surface format is not supported");

	if (!_Capabilities.IsPresentModeSupported(_PresentMode))
		throw std::runtime_error("Swapchain::Swapchain(): Present mode is not supported");

	_Width = _Capabilities.GetCurrentWidth();
	_Height = _Capabilities.GetCurrentHeight();

	if (frame_count != INVALID_ID)
		_FrameCount = frame_count;
	else if (_Capabilities.GetMinFrameCount() > 3)
		_FrameCount = _Capabilities.GetMinFrameCount() + 1;
	else
		_FrameCount = 3;

	if (_FrameCount > _Capabilities.GetMaxFrameCount())
		_FrameCount = _Capabilities.GetMaxFrameCount();

	VkSwapchainCreateInfoKHR vk_swapchain_creation_info{};
	vk_swapchain_creation_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vk_swapchain_creation_info.surface = _WindowSurface->GetVKSurfaceKHR();
	vk_swapchain_creation_info.minImageCount = _FrameCount;
	vk_swapchain_creation_info.imageFormat = _SurfaceFormat.format;
	vk_swapchain_creation_info.imageColorSpace = _SurfaceFormat.colorSpace;
	vk_swapchain_creation_info.imageExtent.width = _Width;
	vk_swapchain_creation_info.imageExtent.height = _Height;
	vk_swapchain_creation_info.imageArrayLayers = 1;
	vk_swapchain_creation_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	vk_swapchain_creation_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vk_swapchain_creation_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vk_swapchain_creation_info.preTransform = _Capabilities.VKSurfaceCapabilities.currentTransform;
	vk_swapchain_creation_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vk_swapchain_creation_info.presentMode = _PresentMode;
	vk_swapchain_creation_info.clipped = VK_TRUE;

	if (old_swapchain.IsValid())
		vk_swapchain_creation_info.oldSwapchain = old_swapchain->GetVKSwapchain();

	VkResult vk_result = vkCreateSwapchainKHR(_LogicalDevice->GetVKLogicalDevice(), &vk_swapchain_creation_info, nullptr, &_VKSwapchain);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Swapchain::Swapchain(): Failed to create swapchain");

	if (old_swapchain.IsValid())
		old_swapchain.DestroyObject();

	vk_result = vkGetSwapchainImagesKHR(_LogicalDevice->GetVKLogicalDevice(), _VKSwapchain, &_FrameCount, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Swapchain::Swapchain(): Failed to get swapchain images");

	List<VkImage> vk_swapchain_images(_FrameCount);
	vk_result = vkGetSwapchainImagesKHR(_LogicalDevice->GetVKLogicalDevice(), _VKSwapchain, &_FrameCount, vk_swapchain_images.Data());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Swapchain::Swapchain(): Failed to get swapchain images");

	_SwapchainTextures.Resize(_FrameCount);
	for (uint32 i = 0; i < _FrameCount; i++)
		_SwapchainTextures[i] = new SwapchainTexture(this, i, vk_swapchain_images[i]);
}

Swapchain::~Swapchain() {
	Destroy();
	NotifyDestruction(this);

	vkDestroySwapchainKHR(_LogicalDevice->GetVKLogicalDevice(), _VKSwapchain, nullptr);
}

/* ---- ---- ---- ---- */

bool Swapchain::AquireNextFrame(uint32& frame_id, const Handle<Semaphore>& signal_semaphore, const Handle<Fence>& signal_fence, uint64 timeout) {
	VkFence vk_signal_fence = VK_NULL_HANDLE;
	if (signal_fence.IsValid())
		vk_signal_fence = signal_fence->GetVKFence();

	VkSemaphore vk_signal_semaphore = VK_NULL_HANDLE;
	if (signal_semaphore.IsValid())
		vk_signal_semaphore = signal_semaphore->GetVKSemaphore();

	frame_id = INVALID_ID;
	VkResult vk_result = vkAcquireNextImageKHR(_LogicalDevice->GetVKLogicalDevice(), _VKSwapchain, timeout, vk_signal_semaphore, vk_signal_fence, &frame_id);
	if (vk_result == VK_SUCCESS)
		return true;

	return false;
}

bool Swapchain::Present(const Handle<CommandQueue>& command_queue, uint32 frame_id, const List<Handle<Semaphore>>& wait_semaphores) {
	List<VkSemaphore> vk_wait_semaphores(wait_semaphores.Length());
	for (uint32 i = 0; i < wait_semaphores.Length(); ++i)
		vk_wait_semaphores[i] = wait_semaphores[i]->GetVKSemaphore();

	VkPresentInfoKHR vk_present_info{};
	vk_present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vk_present_info.waitSemaphoreCount = vk_wait_semaphores.Length();
	vk_present_info.pWaitSemaphores = vk_wait_semaphores.Data();
	vk_present_info.swapchainCount = 1;
	vk_present_info.pSwapchains = &_VKSwapchain;
	vk_present_info.pImageIndices = &frame_id;

	VkResult vk_result = vkQueuePresentKHR(command_queue->GetVKCommandQueue(), &vk_present_info);
	if (vk_result == VK_SUCCESS)
		return true;

	return false;
}
