#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class WindowSurface : public Object {
public:
	struct Capabilities {
		Capabilities() = default;
		Capabilities(const Handle<WindowSurface>& window_surface, const Handle<PhysicalDevice>& physical_device);

		bool IsFormatSupported(VkFormat format, VkColorSpaceKHR color_space);
		bool IsFormatSupported(VkSurfaceFormatKHR format);
		bool IsPresentModeSupported(VkPresentModeKHR mode);

		uint32 GetCurrentWidth() const { return VKSurfaceCapabilities.currentExtent.width; }
		uint32 GetCurrentHeight() const { return VKSurfaceCapabilities.currentExtent.height; }

		uint32 GetMinFrameCount() const { return VKSurfaceCapabilities.minImageCount; }
		uint32 GetMaxFrameCount() const { return VKSurfaceCapabilities.maxImageCount; }

		VkSurfaceCapabilitiesKHR VKSurfaceCapabilities;
		List<VkSurfaceFormatKHR> VKSurfaceFormats;
		List<VkPresentModeKHR> VKPresentModes;
	};

	/* ---- ---- ---- ---- */
public:
	static Handle<WindowSurface> Create(const Handle<Instance>& instance,
										const Handle<Window>& window) {
		return new WindowSurface(instance, window);
	}

private:
	WindowSurface(const Handle<Instance>& instance,
				  const Handle<Window>& window);

public:
	~WindowSurface() override;

	/* ---- ---- ---- ---- */

	VkSurfaceKHR GetVKSurfaceKHR() const { return _VKSurfaceKHR; }

	/* ---- ---- ---- ---- */

	const ParentHandle<Instance>& GetInstance() const { return _Instance; }
	const ParentHandle<Window>& GetWindow() const { return _Window; }

private:
	ParentHandle<Instance> _Instance;
	ParentHandle<Window> _Window;

	/* ---- ---- ---- ---- */

	VkSurfaceKHR _VKSurfaceKHR;
};