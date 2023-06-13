#include "WindowSurface.hpp"

#include "External/GLFW.hpp"
#include "Graphics/Devices/Instance.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"
#include "Graphics/Windows/Window.hpp"

WindowSurface::Capabilities::Capabilities(const Handle<WindowSurface>& window_surface, const Handle<PhysicalDevice>& physical_device) {
	if (window_surface.IsInvalid())
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Window surface is null");

	if (physical_device.IsInvalid())
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Physical device is null");

	/* Query of surface capabilities */
	VkResult vk_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device->GetVKPhysicalDevice(), window_surface->GetVKSurfaceKHR(), &VKSurfaceCapabilities);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Failed to query surface capabilities");

	/* Query of surface formats */
	uint32 vk_surface_format_count = 0;
	vk_result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVKPhysicalDevice(), window_surface->GetVKSurfaceKHR(), &vk_surface_format_count, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Failed to query surface formats");

	VKSurfaceFormats.Resize(vk_surface_format_count);
	vk_result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVKPhysicalDevice(), window_surface->GetVKSurfaceKHR(), &vk_surface_format_count, VKSurfaceFormats.Data());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Failed to query surface formats");

	/* Query of present modes */
	uint32 vk_present_mode_count = 0;
	vk_result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVKPhysicalDevice(), window_surface->GetVKSurfaceKHR(), &vk_present_mode_count, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Failed to query present modes");

	VKPresentModes.Resize(vk_present_mode_count);
	vk_result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVKPhysicalDevice(), window_surface->GetVKSurfaceKHR(), &vk_present_mode_count, VKPresentModes.Data());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("WindowSurface::Capabilities::Capabilities(): Failed to query present modes");
}

bool WindowSurface::Capabilities::IsFormatSupported(VkFormat format, VkColorSpaceKHR color_space) {
    for(uint32 i = 0; i < VKSurfaceFormats.Length(); i++)
        if(VKSurfaceFormats[i].format == format && VKSurfaceFormats[i].colorSpace == color_space)
            return true;

    return false;
}

bool WindowSurface::Capabilities::IsFormatSupported(VkSurfaceFormatKHR format) {
    return IsFormatSupported(format.format, format.colorSpace);
}

bool WindowSurface::Capabilities::IsPresentModeSupported(VkPresentModeKHR mode) {
    for(uint32 i = 0; i < VKPresentModes.Length(); i++)
        if(VKPresentModes[i] == mode)
            return true;

    return false;
}

/* ---- ---- ---- ---- */

WindowSurface::WindowSurface(const Handle<Instance>& instance,
							 const Handle<Window>& window)
	: _Instance(instance, this)
	, _Window(window, this) {
	NotifyCreation(this);

	if (_Instance.IsInvalid())
		throw std::runtime_error("WindowSurface::WindowSurface(): Instance is null");

	if (_Window.IsInvalid())
		throw std::runtime_error("WindowSurface::WindowSurface(): Window is null");

	VkResult vk_result = glfwCreateWindowSurface(instance->GetVKInstance(), window->GetGLFWWindow(), nullptr, &_VKSurfaceKHR);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("WindowSurface::WindowSurface(): Failed to create window surface");
}

WindowSurface::~WindowSurface() {
	Destroy();
	NotifyDestruction(this);

	vkDestroySurfaceKHR(_Instance->GetVKInstance(), _VKSurfaceKHR, nullptr);
}

/* ---- ---- ---- ---- */
