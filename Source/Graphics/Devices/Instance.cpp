#include "Instance.hpp"

#include "PhysicalDevice.hpp"

Instance::CreationInfo::CreationInfo(const List<std::string>& extentions, std::string application_name, std::string engine_name, uint32 application_version, uint32 engine_version, uint32 api_version)
	: InstanceExtentions(extentions)
	, ApplicationName(application_name)
	, EngineName(engine_name)
	, ApplicationVersion(application_version)
	, EngineVersion(engine_version)
	, VulkanAPIVersion(api_version) {
}

Instance::Instance(const Handle<GraphicsContext>& graphics_context, const Instance::CreationInfo& creation_info)
	: Object(graphics_context)
	, Info(creation_info) {
	VkResult vk_result;

	if (graphics_context.IsInvalid())
		throw std::runtime_error("Invalid graphics context");

	NotifyCreation(this);

	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = Info.ApplicationName.c_str();
	app_info.applicationVersion = Info.ApplicationVersion;
	app_info.pEngineName = Info.EngineName.c_str();
	app_info.engineVersion = Info.EngineVersion;
	app_info.apiVersion = Info.ApplicationVersion;

	VkInstanceCreateInfo instance_creation_info{};
	instance_creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_creation_info.pNext = nullptr;
	instance_creation_info.pApplicationInfo = &app_info;

	List<const char*> extention_names = Info.InstanceExtentions.GetCharPointerList();
	instance_creation_info.enabledExtensionCount = extention_names.GetLength();
	instance_creation_info.ppEnabledExtensionNames = extention_names.GetPointer();

	vk_result = vkCreateInstance(&instance_creation_info, nullptr, &VKInstance);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance");

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
