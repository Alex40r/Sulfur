#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class WindowSurface : public Object<Instance, Window> {
public:
	struct Capabilities {
		Capabilities() = default;
		Capabilities(const Handle<WindowSurface>& surface, const Handle<PhysicalDevice>& physical_device);

		VkSurfaceCapabilitiesKHR SurfaceCapabilities = {};
		List<VkSurfaceFormatKHR> SupportedFormats;
		List<VkPresentModeKHR> SupportedPresentModes;

		bool IsFormatSupported(VkFormat format, VkColorSpaceKHR color_space);
		bool IsFormatSupported(VkSurfaceFormatKHR format);
		bool IsPresentModeSupported(VkPresentModeKHR mode);
	};

	static Handle<WindowSurface> Create(const Handle<Instance>& instance,
										const Handle<Window>& window) {
		return new WindowSurface(instance, window);
	}

private:
	WindowSurface(const Handle<Instance>& instance,
				  const Handle<Window>& window);

public:
	~WindowSurface();

	WindowSurface::Capabilities GetCapabilities(const Handle<PhysicalDevice>& physical_device);
	VkSurfaceKHR GetVKSurfaceKHR() { return VKSurfaceKHR; }

private:
	VkSurfaceKHR VKSurfaceKHR;
};