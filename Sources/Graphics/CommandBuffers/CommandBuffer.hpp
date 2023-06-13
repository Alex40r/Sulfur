#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class CommandBuffer : public Object {
public:
	static Handle<CommandBuffer> Create(const Handle<CommandPool>& command_pool,
										VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
		return new CommandBuffer(command_pool, level);
	}

private:
	CommandBuffer(const Handle<CommandPool>& command_pool,
				  VkCommandBufferLevel level);

public:
	~CommandBuffer() override;

	/* ---- ---- ---- ---- */

	VkCommandBuffer GetVKCommandBuffer() const { return _VKCommandBuffer; }

	void Reset();
	void BeginRecording();
	void FinishRecording();

	void BeginRenderPass(const Handle<RenderPass>& render_pass,
						 const Handle<Framebuffer>& framebuffer,
						 uint32 area_width,
						 uint32 area_height,
						 const List<VkClearValue>& clear_values = {},
						 VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);

	void EndRenderPass();

	/* ---- ---- ---- ---- */

	const ParentHandle<CommandPool>& GetCommandPool() const { return _CommandPool; }

private:
	ParentHandle<CommandPool> _CommandPool;

	/* ---- ---- ---- ---- */

	VkCommandBuffer _VKCommandBuffer;
};