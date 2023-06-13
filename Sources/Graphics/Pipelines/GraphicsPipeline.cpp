#include "GraphicsPipeline.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/RenderPasses/RenderPass.hpp"
#include "Graphics/RenderPasses/RenderSubpass.hpp"
#include "PipelineLayout.hpp"

GraphicsPipeline::GraphicsPipeline(const Handle<LogicalDevice>& logical_device,
								   const Handle<PipelineLayout>& pipeline_layout,
								   const Handle<RenderPass>& render_pass,
								   const Handle<RenderSubpass>& render_subpass,
								   const List<Shader::Stage>& stages)
	: _LogicalDevice(logical_device, this)
	, _PipelineLayout(pipeline_layout, this)
	, _RenderPass(render_pass, this)
	, _RenderSubpass(render_subpass, this) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("GraphicsPipeline::GraphicsPipeline(): Logical device is null");

	if (_PipelineLayout.IsInvalid())
		throw std::runtime_error("GraphicsPipeline::GraphicsPipeline(): Pipeline layout is null");

	if (_RenderPass.IsInvalid())
		throw std::runtime_error("GraphicsPipeline::GraphicsPipeline(): Render pass is null");

	if (_RenderSubpass.IsInvalid())
		throw std::runtime_error("GraphicsPipeline::GraphicsPipeline(): Render subpass is null");

	List<VkPipelineShaderStageCreateInfo> vk_stages(stages.Length());
	for (uint32 i = 0; i < vk_stages.Length(); i++)
		stages[i].GetVKStageCreationInfo(vk_stages[i]);

	/* ---- ---- ---- ---- */
	/* ---- ---- ---- ---- */
	/* ---- ---- ---- ---- */

	VkPipelineVertexInputStateCreateInfo vertex_input{};
	vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	// TODO : BETTER IMPLEMENTATION
	VkVertexInputBindingDescription vertex_binding{};
	vertex_binding.binding = 0;
	vertex_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertex_binding.stride = 0;

	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewport_state{};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState color_blend_attachment{};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;

	VkPipelineDepthStencilStateCreateInfo depth_stencil{};
	depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil.depthTestEnable = VK_FALSE;
	depth_stencil.depthWriteEnable = VK_FALSE;
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_stencil.depthBoundsTestEnable = VK_FALSE;
	depth_stencil.minDepthBounds = 0.0f;
	depth_stencil.maxDepthBounds = 1.0f;
	depth_stencil.stencilTestEnable = VK_FALSE;

	VkPipelineDynamicStateCreateInfo dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};
	dynamic_state.dynamicStateCount = 2; // TODO : BETTER
	dynamic_state.pDynamicStates = dynamicStates;

	/* ---- ---- ---- ---- */
	/* ---- ---- ---- ---- */
	/* ---- ---- ---- ---- */

	VkGraphicsPipelineCreateInfo vk_graphics_pipeline_creation_info{};
	vk_graphics_pipeline_creation_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	vk_graphics_pipeline_creation_info.stageCount = vk_stages.Length();
	vk_graphics_pipeline_creation_info.pStages = vk_stages.Data();
	vk_graphics_pipeline_creation_info.layout = _PipelineLayout->GetVKPipelineLayout();
	vk_graphics_pipeline_creation_info.renderPass = _RenderPass->GetVKRenderPass();
	vk_graphics_pipeline_creation_info.basePipelineHandle = VK_NULL_HANDLE;
	vk_graphics_pipeline_creation_info.basePipelineIndex = -1;

	vk_graphics_pipeline_creation_info.subpass = _RenderPass->GetSubpassID(_RenderSubpass);
	if (vk_graphics_pipeline_creation_info.subpass == INVALID_ID)
		throw std::runtime_error("GraphicsPipeline::GraphicsPipeline(): Render subpass is not part of the render pass");

	vk_graphics_pipeline_creation_info.pVertexInputState = &vertex_input;
	vk_graphics_pipeline_creation_info.pInputAssemblyState = &input_assembly;
	vk_graphics_pipeline_creation_info.pViewportState = &viewport_state;
	vk_graphics_pipeline_creation_info.pRasterizationState = &rasterizer;
	vk_graphics_pipeline_creation_info.pMultisampleState = &multisampling;
	vk_graphics_pipeline_creation_info.pDepthStencilState = &depth_stencil;
	vk_graphics_pipeline_creation_info.pColorBlendState = &color_blending;
	vk_graphics_pipeline_creation_info.pDynamicState = &dynamic_state;

	VkResult vk_result = vkCreateGraphicsPipelines(_LogicalDevice->GetVKLogicalDevice(), VK_NULL_HANDLE, 1, &vk_graphics_pipeline_creation_info, nullptr, &_VKPipeline);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("GraphicsPipeline::GraphicsPipeline(): Failed to create graphics pipeline");
}

GraphicsPipeline::~GraphicsPipeline() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyPipeline(_LogicalDevice->GetVKLogicalDevice(), _VKPipeline, nullptr);
}

/* ---- ---- ---- ---- */