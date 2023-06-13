#include "Buffer.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Memory/Memory.hpp"

Buffer::Buffer(const Handle<LogicalDevice>& logical_device,
			   uint64 size,
			   VkBufferUsageFlags usage,
			   VkBufferCreateFlags flags)
	: _LogicalDevice(logical_device, this)
	, _Size(size)
	, _Usage(usage)
	, _Flags(flags) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("Buffer::Buffer(): logical_device is invalid");

	VkBufferCreateInfo vk_buffer_creation_info{};
	vk_buffer_creation_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vk_buffer_creation_info.size = _Size;
	vk_buffer_creation_info.usage = _Usage;
	vk_buffer_creation_info.flags = _Flags;

	VkResult vk_result = vkCreateBuffer(_LogicalDevice->GetVKLogicalDevice(), &vk_buffer_creation_info, nullptr, &_VKBuffer);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Buffer::Buffer(): Failed to create buffer");

	vkGetBufferMemoryRequirements(_LogicalDevice->GetVKLogicalDevice(), _VKBuffer, &_VKMemoryRequirements);
}

Buffer::~Buffer() {
	Destroy();
	NotifyDestruction(this);

	vkDestroyBuffer(_LogicalDevice->GetVKLogicalDevice(), _VKBuffer, nullptr);
}

/* ---- ---- ---- ---- */

void Buffer::Bind(const Handle<Memory>& memory, uint64 offset) {
	VkResult vk_result = vkBindBufferMemory(_LogicalDevice->GetVKLogicalDevice(), _VKBuffer, memory->GetVKDeviceMemory(), offset);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Buffer::Bind(): Failed to bind buffer");

	_BindMemory.Set(memory, this);
	_BindOffset = offset;
}

IResource::Requirements Buffer::GetRequirements() const {
	return IResource::Requirements(_VKMemoryRequirements);
}
