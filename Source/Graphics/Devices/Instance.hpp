#pragma once

#include "External/Vulkan.hpp"
#include "Utils/Utils.hpp"

class Instance : public Object<GraphicsContext> {
public:
	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(const List<std::string>& extentions,
					 std::string application_name,
					 std::string engine_name,
					 uint32 application_version,
					 uint32 engine_version,
					 uint32 api_version);

		List<std::string> InstanceExtentions;
		std::string ApplicationName;
		std::string EngineName;
		uint32 ApplicationVersion;
		uint32 EngineVersion;
		uint32 VulkanAPIVersion;
	};

	static Handle<Instance> Create(const Handle<GraphicsContext>& graphics_context,
								   const List<std::string>& extentions,
								   std::string application_name,
								   std::string engine_name,
								   uint32 application_version = VK_MAKE_API_VERSION(0, 1, 0, 0),
								   uint32 engine_version = VK_MAKE_API_VERSION(0, 1, 0, 0),
								   uint32 api_version = VK_API_VERSION_1_0) {
		return new Instance(graphics_context, Instance::CreationInfo(extentions, application_name, engine_name, application_version, engine_version, api_version));
	}
	static Handle<Instance> Create(const Handle<GraphicsContext>& graphics_context,
								   const Instance::CreationInfo& creation_info) {
		return new Instance(graphics_context, creation_info);
	}

private:
	Instance(const Handle<GraphicsContext>& graphics_context,
			 const Instance::CreationInfo& creation_info);

public:
	~Instance();

	const Instance::CreationInfo& GetCreationInfo() { return Info; }

	const List<Handle<PhysicalDevice>>& GetPhysicalDevices() { return PhysicalDevices; }

	VkInstance GetVKInstance() { return VKInstance; }

private:
	Instance::CreationInfo Info;

	VkInstance VKInstance;

	List<Handle<PhysicalDevice>> PhysicalDevices;
};