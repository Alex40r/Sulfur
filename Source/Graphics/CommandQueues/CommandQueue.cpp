#include "CommandQueue.hpp"

CommandQueue::CommandQueue(const Handle<LogicalDevice>& logical_device,
						   const Handle<CommandQueueFamily>& command_queue_family,
						   VkQueue vk_queue,
						   uint32 queue_index)
	: Parent<LogicalDevice>(logical_device)
	, Parent<CommandQueueFamily>(command_queue_family)
	, VKQueue(vk_queue)
	, QueueID(queue_index) {
	NotifyCreation(this);

	if (Parent<LogicalDevice>::Get().IsInvalid())
		throw std::runtime_error("CommandQueue must be created with a valid LogicalDevice");

	if (Parent<CommandQueueFamily>::Get().IsInvalid())
		throw std::runtime_error("CommandQueue must be created with a valid CommandQueueFamily");
}

CommandQueue::~CommandQueue() {
	DestroyChildren();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */