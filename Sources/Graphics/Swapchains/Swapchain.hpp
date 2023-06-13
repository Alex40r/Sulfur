#pragma once

#include "Utils/Utils.hpp"

#include "WindowSurface.hpp"

class Swapchain : public Object {
public:
	static Handle<Swapchain> Create(const Handle<LogicalDevice>& logical_device,
									const Handle<WindowSurface>& window_surface,
									VkFormat format,
									VkColorSpaceKHR color_space,
									VkPresentModeKHR present_mode,
									const Handle<Swapchain>& old_swapchain = nullptr,
									uint32 frame_count = INVALID_ID) {
		return new Swapchain(logical_device, window_surface, format, color_space, present_mode, old_swapchain, frame_count);
	}

private:
	Swapchain(const Handle<LogicalDevice>& logical_device,
			  const Handle<WindowSurface>& window_surface,
			  VkFormat format,
			  VkColorSpaceKHR color_space,
			  VkPresentModeKHR present_mode,
			  const Handle<Swapchain>& old_swapchain,
			  uint32 frame_count);

public:
	~Swapchain() override;

	/* ---- ---- ---- ---- */

	VkSwapchainKHR GetVKSwapchain() const { return _VKSwapchain; }

	VkSurfaceFormatKHR GetSurfaceFormat() const { return _SurfaceFormat; }
	VkPresentModeKHR GetPresentMode() const { return _PresentMode; }

	uint32 GetWidth() const { return _Width; }
	uint32 GetHeight() const { return _Height; }
	uint32 GetFrameCount() const { return _FrameCount; }

	bool AquireNextFrame(uint32& frame_id, const Handle<Semaphore>& signal_semaphore = nullptr, const Handle<Fence>& signal_fence = nullptr, uint64 timeout = -1);
	bool Present(const Handle<CommandQueue>& command_queue, uint32 frame_id, const List<Handle<Semaphore>>& wait_semaphores = {});

	const List<Handle<SwapchainTexture>>& GetSwapchainTextures() const { return _SwapchainTextures; }
	const Handle<SwapchainTexture>& GetSwapchainTexture(uint32 frame_id) const { return _SwapchainTextures[frame_id]; }
	uint32 GetSwapchainTextureCount() const { return _SwapchainTextures.Length(); }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<WindowSurface>& GetWindowSurface() const { return _WindowSurface; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<WindowSurface> _WindowSurface;

	/* ---- ---- ---- ---- */

	VkSwapchainKHR _VKSwapchain;

	WindowSurface::Capabilities _Capabilities;

	VkSurfaceFormatKHR _SurfaceFormat;
	VkPresentModeKHR _PresentMode;
	uint32 _Width;
	uint32 _Height;
	uint32 _FrameCount;

	List<Handle<SwapchainTexture>> _SwapchainTextures;
};