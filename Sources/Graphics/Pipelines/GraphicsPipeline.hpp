#pragma once

#include "Utils/Utils.hpp"

#include "IPipeline.hpp"
#include "Shader.hpp"

class GraphicsPipeline : public IPipeline {
public:
	static Handle<GraphicsPipeline> Create(const Handle<LogicalDevice>& logical_device,
										   const Handle<PipelineLayout>& pipeline_layout,
										   const Handle<RenderPass>& render_pass,
										   const Handle<RenderSubpass>& render_subpass,
										   const List<Shader::Stage>& stages) {
		return new GraphicsPipeline(logical_device, pipeline_layout, render_pass, render_subpass, stages);
	}
	/* ---- ---- ---- ---- */

private:
	GraphicsPipeline(const Handle<LogicalDevice>& logical_device,
					 const Handle<PipelineLayout>& pipeline_layout,
					 const Handle<RenderPass>& render_pass,
					 const Handle<RenderSubpass>& render_subpass,
					 const List<Shader::Stage>& stages);

public:
	~GraphicsPipeline() override;

	/* ---- ---- ---- ---- */

	VkPipeline GetVKPipeline() const override { return _VKPipeline; }

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<PipelineLayout>& GetPipelineLayout() const { return _PipelineLayout; }
	const ParentHandle<RenderPass>& GetRenderPass() const { return _RenderPass; }
    const ParentHandle<RenderSubpass>& GetRenderSubpass() const { return _RenderSubpass; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<PipelineLayout> _PipelineLayout;
	ParentHandle<RenderPass> _RenderPass;
    ParentHandle<RenderSubpass> _RenderSubpass;

	/* ---- ---- ---- ---- */

	VkPipeline _VKPipeline;
};