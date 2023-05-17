#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "ICommandBuffer.hpp"

class CommandBuffer : public ICommandBuffer, Parent<CommandPool> {
public:
	static Handle<CommandBuffer> Create(const Handle<CommandPool> command_pool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
		return new CommandBuffer(command_pool, level);
	}

private:
	CommandBuffer(const Handle<CommandPool> command_pool, VkCommandBufferLevel level);

public:
	~CommandBuffer() override;

	Handle<CommandBuffer> GetCommandBuffer() override { return this; }

	/* ---- ---- ---- ---- */

	void Reset();
	void BeginRecording();
	void EndRecording();

	VkCommandBuffer GetVKCommandBuffer() { return VKCommandBuffer; }

	/* ---- ---- ---- ---- */
private:
	VkCommandBuffer VKCommandBuffer;
};