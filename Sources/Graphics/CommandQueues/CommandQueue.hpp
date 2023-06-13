#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

class CommandQueue : public Object {
	friend class LogicalDevice;

public:
	struct CreationInfo {
		CreationInfo() = default;
		CreationInfo(uint32 family_id,
					 float priority = 1.0F);
		CreationInfo(const Handle<CommandQueueFamily>& family,
					 float priority = 1.0F);

		/* ---- ---- ---- ---- */

		void GetVKQueueCreationInfo(VkDeviceQueueCreateInfo& vk_queue_creation_info) const;

		/* ---- ---- ---- ---- */

		uint32 FamilyID = INVALID_ID;
		float Priority = 1.0F;
	};

	/* ---- ---- ---- ---- */
private:
	CommandQueue(const Handle<LogicalDevice>& logical_device,
				 const Handle<CommandQueueFamily>& command_queue_family,
				 uint32 command_queue_id,
				 VkQueue vk_command_queue);

public:
	~CommandQueue() override;

	/* ---- ---- ---- ---- */

	VkQueue GetVKCommandQueue() const { return _VKCommandQueue; }

	uint32 GetQueueID() const { return _CommandQueueID; }

	void Submit(const List<Handle<CommandBuffer>>& command_buffers,
				const List<Handle<Semaphore>>& wait_semaphores = {},
				const List<VkPipelineStageFlags>& wait_stages = {},
				const List<Handle<Semaphore>>& signal_semaphores = {},
				const Handle<Fence>& signal_fence = nullptr);

	/* ---- ---- ---- ---- */

	const ParentHandle<LogicalDevice>& GetLogicalDevice() const { return _LogicalDevice; }
	const ParentHandle<CommandQueueFamily>& GetCommandQueueFamily() const { return _CommandQueueFamily; }

private:
	ParentHandle<LogicalDevice> _LogicalDevice;
	ParentHandle<CommandQueueFamily> _CommandQueueFamily;

	/* ---- ---- ---- ---- */

	uint32 _CommandQueueID;

	VkQueue _VKCommandQueue;
};