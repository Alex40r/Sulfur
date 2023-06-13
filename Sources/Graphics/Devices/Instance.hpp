#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class Instance : public Object {
public:
	static Handle<Instance> Create(const Handle<GraphicsContext>& graphics_context,
								   const List<std::string>& extensions,
								   const std::string& application_name,
								   uint32 application_version = VK_MAKE_API_VERSION(0, 1, 0, 0),
								   uint32 api_version = VK_API_VERSION_1_0) {
		return new Instance(graphics_context, extensions, application_name, application_version, api_version);
	}

private:
	Instance(const Handle<GraphicsContext>& graphics_context,
			 const List<std::string>& extensions,
			 const std::string& application_name,
			 uint32 application_version,
			 uint32 api_version);

public:
	~Instance() override;

	/* ---- ---- ---- ---- */

	VkInstance GetVKInstance() const { return _VKInstance; }

	const List<Handle<PhysicalDevice>>& GetPhysicalDevices() const { return _PhysicalDevices; }
	const Handle<PhysicalDevice>& GetPhysicalDevice(uint32 index) const { return _PhysicalDevices[index]; }
	uint32 GetPhysicalDeviceCount() const { return _PhysicalDevices.Length(); }

	const std::string& GetApplicationName() const { return _ApplicationName; }
	uint32 GetApplicationVersion() const { return _ApplicationVersion; }

	std::string GetEngineName() const { return ENGINE_NAME; }
	uint32 GetEngineVersion() const { return ENGINE_VERSION; }

	uint32 GetAPIVersion() const { return _APIVersion; }

	/* ---- ---- ---- ---- */

	const ParentHandle<GraphicsContext>& GetGraphicsContext() const { return _GraphicsContext; }

private:
	ParentHandle<GraphicsContext> _GraphicsContext;

	/* ---- ---- ---- ---- */

	VkInstance _VKInstance;

	List<Handle<PhysicalDevice>> _PhysicalDevices;

	std::string _ApplicationName;
	uint32 _ApplicationVersion;
	uint32 _APIVersion;
};