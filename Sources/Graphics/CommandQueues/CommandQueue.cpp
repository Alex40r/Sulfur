#include "CommandQueue.hpp"

#include "CommandQueueFamily.hpp"
#include "Graphics/Synchronizations/Fence.hpp"
#include "Graphics/Synchronizations/Semaphore.hpp"
#include "Graphics/CommandBuffers/CommandBuffer.hpp"

CommandQueue::CreationInfo::CreationInfo(uint32 family_id, float priority)
	: FamilyID(family_id)
	, Priority(priority) {
}

CommandQueue::CreationInfo::CreationInfo(const Handle<CommandQueueFamily>& family, float priority)
	: Priority(priority) {
	if (family.IsValid())
		FamilyID = family->GetFamilyID();
}

void CommandQueue::CreationInfo::GetVKQueueCreationInfo(VkDeviceQueueCreateInfo& vk_queue_creation_info) const {
	vk_queue_creation_info = {};

	vk_queue_creation_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vk_queue_creation_info.queueFamilyIndex = FamilyID;
	vk_queue_creation_info.queueCount = 1;
	vk_queue_creation_info.pQueuePriorities = &Priority;
}

/* ---- ---- ---- ---- */

CommandQueue::CommandQueue(const Handle<LogicalDevice>& logical_device,
						   const Handle<CommandQueueFamily>& command_queue_family,
						   uint32 command_queue_id,
						   VkQueue vk_command_queue)
	: _LogicalDevice(logical_device, this)
	, _CommandQueueFamily(command_queue_family, this)
	, _CommandQueueID(command_queue_id)
	, _VKCommandQueue(vk_command_queue) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("CommandQueue::CommandQueue(): Logical device is null");

	if (_CommandQueueFamily.IsInvalid())
		throw std::runtime_error("CommandQueue::CommandQueue(): Command queue family is null");
}

CommandQueue::~CommandQueue() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void CommandQueue::Submit(const List<Handle<CommandBuffer>>& command_buffers, const List<Handle<Semaphore>>& wait_semaphores, const List<VkPipelineStageFlags>& wait_stages, const List<Handle<Semaphore>>& signal_semaphores, const Handle<Fence>& signal_fence) {
	if (command_buffers.Empty())
		return;

	if (wait_semaphores.Length() != wait_stages.Length())
		throw std::runtime_error("CommandQueue::Submit(): Different number of wait semaphores and wait stages");

	List<VkCommandBuffer> vk_command_buffers(command_buffers.Length());

	for (uint32 i = 0; i < command_buffers.Length(); ++i) {
		if (command_buffers[i].IsInvalid())
			throw std::runtime_error("CommandQueue::Submit(): Command buffer is null");

		vk_command_buffers[i] = command_buffers[i]->GetVKCommandBuffer();
	}

	List<VkSemaphore> vk_wait_semaphores(wait_semaphores.Length());

	for (uint32 i = 0; i < wait_semaphores.Length(); ++i) {
		if (wait_semaphores[i].IsInvalid())
			throw std::runtime_error("CommandQueue::Submit(): Wait semaphore is null");

		vk_wait_semaphores[i] = wait_semaphores[i]->GetVKSemaphore();
	}

	List<VkSemaphore> vk_signal_semaphores(signal_semaphores.Length());

	for (uint32 i = 0; i < signal_semaphores.Length(); ++i) {
		if (signal_semaphores[i].IsInvalid())
			throw std::runtime_error("CommandQueue::Submit(): Signal semaphore is null");

		vk_signal_semaphores[i] = signal_semaphores[i]->GetVKSemaphore();
	}

	VkSubmitInfo vk_submit_info{};
	vk_submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	vk_submit_info.waitSemaphoreCount = vk_wait_semaphores.Length();
	vk_submit_info.pWaitSemaphores = vk_wait_semaphores.Data();
	vk_submit_info.pWaitDstStageMask = wait_stages.Data();

	vk_submit_info.commandBufferCount = vk_command_buffers.Length();
	vk_submit_info.pCommandBuffers = vk_command_buffers.Data();

	vk_submit_info.signalSemaphoreCount = vk_signal_semaphores.Length();
	vk_submit_info.pSignalSemaphores = vk_signal_semaphores.Data();


	VkFence vk_fence = VK_NULL_HANDLE;
	if (signal_fence.IsValid())
		vk_fence = signal_fence->GetVKFence();

	VkResult vk_result = vkQueueSubmit(_VKCommandQueue, 1, &vk_submit_info, vk_fence);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("CommandQueue::Submit(): Failed to submit command buffers");
}