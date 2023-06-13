#include "Instance.hpp"

#include "PhysicalDevice.hpp"

Instance::Instance(const Handle<GraphicsContext>& graphics_context,
				   const List<std::string>& extensions,
				   const std::string& application_name,
				   uint32 application_version,
				   uint32 api_version)
	: _GraphicsContext(graphics_context, this)
	, _ApplicationName(application_name)
	, _ApplicationVersion(application_version)
	, _APIVersion(api_version) {
	NotifyCreation(this);

	if (_GraphicsContext.IsInvalid())
		throw std::runtime_error("Instance::Instance(): Graphics context is null");

	VkApplicationInfo vk_application_info{};
	vk_application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vk_application_info.applicationVersion = application_version;
	vk_application_info.pApplicationName = application_name.c_str();
	vk_application_info.engineVersion = ENGINE_VERSION;
	vk_application_info.pEngineName = ENGINE_NAME;
	vk_application_info.apiVersion = api_version;

	List<const char*> vk_extensions(extensions.Length());
	for (uint32 i = 0; i < extensions.Length(); i++)
		vk_extensions[i] = extensions[i].c_str();

	VkInstanceCreateInfo vk_instance_create_info{};
	vk_instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vk_instance_create_info.pApplicationInfo = &vk_application_info;
	vk_instance_create_info.enabledExtensionCount = vk_extensions.Length();
	vk_instance_create_info.ppEnabledExtensionNames = vk_extensions.Data();

	VkResult vk_result = vkCreateInstance(&vk_instance_create_info, nullptr, &_VKInstance);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Instance::Instance(): Failed to create vulkan instance");

	uint32 physical_device_count;
	vk_result = vkEnumeratePhysicalDevices(_VKInstance, &physical_device_count, nullptr);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Instance::Instance(): Failed to enumerate physical devices");

	List<VkPhysicalDevice> vk_physical_devices(physical_device_count);
	vk_result = vkEnumeratePhysicalDevices(_VKInstance, &physical_device_count, vk_physical_devices.Data());
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Instance::Instance(): Failed to enumerate physical devices");

	_PhysicalDevices.Resize(physical_device_count);
	for (uint32 i = 0; i < physical_device_count; i++)
		_PhysicalDevices[i] = new PhysicalDevice(this, i, vk_physical_devices[i]);
}

Instance::~Instance() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyInstance(_VKInstance, nullptr);
}

/* ---- ---- ---- ---- */