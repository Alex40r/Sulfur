#include "Instance.hpp"

#include "PhysicalDevice.hpp"

Instance::Instance(const Handle<GraphicsContext>& graphics_context,
				   List<std::string> extensions,
				   std::string application_name,
				   std::string engine_name,
				   uint32 application_version,
				   uint32 engine_version,
				   uint32 api_version)
	: Parent<GraphicsContext>(graphics_context) {
	NotifyCreation(this);

	if (Parent<GraphicsContext>::Get().IsInvalid())
		throw std::runtime_error("Instance must be created with a valid GraphicsContext");

	VkResult vk_result;

	VkApplicationInfo application_info{};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.applicationVersion = application_version;
	application_info.pApplicationName = application_name.c_str();
	application_info.engineVersion = engine_version;
	application_info.pEngineName = engine_name.c_str();
	application_info.apiVersion = api_version;

	List<const char*> extension_names;
	extension_names.Resize(extensions.GetLength());
	for (uint32 i = 0; i < extensions.GetLength(); i++)
		extension_names[i] = extensions[i].c_str();

	VkInstanceCreateInfo instance_info{};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &application_info;
	instance_info.enabledExtensionCount = extension_names.GetLength();
	instance_info.ppEnabledExtensionNames = extension_names.GetPointer();

	vk_result = vkCreateInstance(&instance_info, nullptr, &VKInstance);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance");

	uint32 physical_device_count;
	vk_result = vkEnumeratePhysicalDevices(VKInstance, &physical_device_count, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to enumerate physical devices");

	List<VkPhysicalDevice> vk_physical_devices(physical_device_count);

	vk_result = vkEnumeratePhysicalDevices(VKInstance, &physical_device_count, vk_physical_devices.GetPointer());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to enumerate physical devices");

	PhysicalDevices.Resize(physical_device_count);

	for (uint32 i = 0; i < physical_device_count; i++)
		PhysicalDevices[i] = PhysicalDevice::Create(this, vk_physical_devices[i]);
}

Instance::~Instance() {
	DestroyChildren();
	NotifyDestruction(this);

    vkDestroyInstance(VKInstance, nullptr);
}

/* ---- ---- ---- ---- */