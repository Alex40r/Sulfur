#include "WindowSurface.hpp"

#include "Graphics/Devices/Instance.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"
#include "Window.hpp"

WindowSurface::Capabilities::Capabilities(const Handle<WindowSurface>& surface, const Handle<PhysicalDevice>& physical_device) {
	if (surface.IsInvalid())
		throw std::runtime_error("Invalid surface");

	if (physical_device.IsInvalid())
		throw std::runtime_error("Invalid physical device");

	VkResult vk_result;

	/* Query of surface capabilities */
	vk_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device->GetVKPhysicalDevice(), surface->GetVKSurfaceKHR(), &SurfaceCapabilities);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Could not query surface capabilities");

	/* Query of surface formats */
	uint32 surface_format_count;
	vk_result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVKPhysicalDevice(), surface->GetVKSurfaceKHR(), &surface_format_count, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Could not query available surface formats");

	SupportedFormats.Resize(surface_format_count);

	vk_result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVKPhysicalDevice(), surface->GetVKSurfaceKHR(), &surface_format_count, SupportedFormats.GetPointer());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Could not query available surface formats");

	/* Query of surface presentation modes */
	uint32 present_mode_count;
	vk_result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVKPhysicalDevice(), surface->GetVKSurfaceKHR(), &present_mode_count, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Could not query surface presentation modes");

	SupportedPresentModes.Resize(present_mode_count);

	vk_result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVKPhysicalDevice(), surface->GetVKSurfaceKHR(), &present_mode_count, SupportedPresentModes.GetPointer());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Could not query surface presentation modes");
}

bool WindowSurface::Capabilities::IsFormatSupported(VkFormat format, VkColorSpaceKHR color_space) {
	for (uint32 i = 0; i < SupportedFormats.GetLength(); i++)
		if (SupportedFormats[i].colorSpace == color_space && SupportedFormats[i].format == format)
			return true;

	return false;
}

bool WindowSurface::Capabilities::IsFormatSupported(VkSurfaceFormatKHR format) {
	return IsFormatSupported(format.format, format.colorSpace);
}

bool WindowSurface::Capabilities::IsPresentModeSupported(VkPresentModeKHR mode) {
	for (uint32 i = 0; i < SupportedPresentModes.GetLength(); i++)
		if (SupportedPresentModes[i] == mode)
			return true;
	return false;
}

WindowSurface::WindowSurface(const Handle<Instance>& instance,
							 const Handle<Window>& window)
	: Object(instance, window) {
	VkResult vk_result;

	if (instance.IsInvalid())
		throw std::runtime_error("Invalid instance");

	if (window.IsInvalid())
		throw std::runtime_error("Invalid window");

	NotifyCreation(this);

	vk_result = glfwCreateWindowSurface(instance->GetVKInstance(), window->GetGLFWWindow(), nullptr, &VKSurfaceKHR);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface");
}

WindowSurface::~WindowSurface() {
	DestroyChildren();
	NotifyDestruction(this);

	vkDestroySurfaceKHR(Utils::GetParent<Instance>(this)->GetVKInstance(), VKSurfaceKHR, nullptr);
}

WindowSurface::Capabilities WindowSurface::GetCapabilities(const Handle<PhysicalDevice>& physical_device) {
	return WindowSurface::Capabilities(this, physical_device);
}
