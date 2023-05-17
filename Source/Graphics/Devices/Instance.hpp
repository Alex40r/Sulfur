#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "IInstance.hpp"

class Instance : public IInstance, Parent<GraphicsContext> {
public:
	static Handle<Instance> Create(const Handle<GraphicsContext>& graphics_context,
								   List<std::string> extensions,
								   std::string application_name,
								   std::string engine_name,
								   uint32 application_version = VK_MAKE_API_VERSION(0, 1, 0, 0),
								   uint32 engine_version = VK_MAKE_API_VERSION(0, 1, 0, 0),
								   uint32 api_version = VK_API_VERSION_1_0) {
		return new Instance(graphics_context, extensions, application_name, engine_name, application_version, engine_version, api_version);
	}

private:
	Instance(const Handle<GraphicsContext>& graphics_context,
			 List<std::string> extensions,
			 std::string application_name,
			 std::string engine_name,
			 uint32 application_version,
			 uint32 engine_version,
			 uint32 api_version);

public:
	~Instance() override;

	Handle<Instance> GetInstance() override { return this; }

	/* ---- ---- ---- ---- */

    const List<Handle<PhysicalDevice>>& GetPhysicalDevices() { return PhysicalDevices; }
    const Handle<PhysicalDevice>& GetPhysicalDevice(uint32 physical_device_id) { return PhysicalDevices[physical_device_id]; }
    uint32 GetPhysicalDeviceCount() { return PhysicalDevices.GetLength(); }

    VkInstance GetVKInstance() { return VKInstance; }

	/* ---- ---- ---- ---- */
private:
	VkInstance VKInstance;

	List<Handle<PhysicalDevice>> PhysicalDevices;
};