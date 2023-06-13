#include "Shader.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"

Shader::Stage::Stage(const Handle<Shader>& stage_shader, VkShaderStageFlagBits vk_stage, std::string entry_point)
	: StageShader(stage_shader)
	, VKStage(vk_stage)
	, EntryPoint(entry_point) {
}

void Shader::Stage::GetVKStageCreationInfo(VkPipelineShaderStageCreateInfo& info) const {
	info = {};

	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.stage = VKStage;
	info.module = StageShader->GetVKShaderModule();
	info.pName = EntryPoint.c_str();
}

/* ---- ---- ---- ---- */

Shader::Shader(const Handle<LogicalDevice>& logical_device,
			   const List<uint8>& code)
	: _LogicalDevice(logical_device, this) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("Shader::Shader(): Logical device is null");

	VkShaderModuleCreateInfo vk_shader_module_creation_info{};
	vk_shader_module_creation_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vk_shader_module_creation_info.codeSize = code.Length();
	vk_shader_module_creation_info.pCode = reinterpret_cast<const uint32*>(code.Data());

	VkResult vk_result = vkCreateShaderModule(_LogicalDevice->GetVKLogicalDevice(), &vk_shader_module_creation_info, nullptr, &_VKShaderModule);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Shader::Shader(): Failed to create shader module");
}

Shader::~Shader() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyShaderModule(_LogicalDevice->GetVKLogicalDevice(), _VKShaderModule, nullptr);
}

/* ---- ---- ---- ---- */
