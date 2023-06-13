#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class Shader : public Object {
public:
	struct Stage {
		Stage() = default;
        Stage(const Handle<Shader>& stage_shader, VkShaderStageFlagBits vk_stage, std::string entry_point);

        Handle<Shader> StageShader;
        VkShaderStageFlagBits VKStage;
        std::string EntryPoint;

        void GetVKStageCreationInfo(VkPipelineShaderStageCreateInfo& info) const;
	};

	/* ---- ---- ---- ---- */
public:
	static Handle<Shader> Create(const Handle<LogicalDevice>& logical_device,
								 const List<uint8>& code) {
		return new Shader(logical_device, code);
	}

private:
	Shader(const Handle<LogicalDevice>& logical_device,
		   const List<uint8>& code);

public:
	~Shader() override;

	/* ---- ---- ---- ---- */

	VkShaderModule GetVKShaderModule() const { return _VKShaderModule; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;

	/* ---- ---- ---- ---- */

	VkShaderModule _VKShaderModule;
};