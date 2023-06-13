#include "Memory.hpp"

#include "Graphics/Devices/LogicalDevice.hpp"
#include "Graphics/Devices/PhysicalDevice.hpp"
#include "Graphics/Resources/IResource.hpp"
#include "MemoryType.hpp"

Memory::Memory(const Handle<LogicalDevice>& logical_device,
			   const Handle<MemoryType>& memory_type,
			   uint64 size)
	: _LogicalDevice(logical_device, this)
	, _MemoryType(memory_type, this)
	, _Size(size) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("Memory::Memory(): Logical device is null");

	if (_MemoryType.IsInvalid())
		throw std::runtime_error("Memory::Memory(): Memory type is null");

	VkMemoryAllocateInfo vk_memory_allocation_info{};
	vk_memory_allocation_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vk_memory_allocation_info.allocationSize = _Size;
	vk_memory_allocation_info.memoryTypeIndex = _MemoryType->GetTypeID();

	VkResult vk_result = vkAllocateMemory(_LogicalDevice->GetVKLogicalDevice(), &vk_memory_allocation_info, nullptr, &_VKDeviceMemory);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Memory::Memory(): Failed to allocate memory");
}

Memory::Memory(const Handle<LogicalDevice>& logical_device,
			   const List<Handle<IResource>>& resources,
			   AllocationType allocation_type,
			   VkMemoryPropertyFlags target_properties)
	: _LogicalDevice(logical_device, this) {
	NotifyCreation(this);

	if (_LogicalDevice.IsInvalid())
		throw std::runtime_error("Memory::Memory(): Logical device is null");

	_Size = 0;
	uint32 memory_type_bits = 0xFFFFFFFF;

	for (uint32 i = 0; i < resources.Length(); i++) {
		const Handle<IResource>& resource = resources[i];

		if (resource.IsInvalid())
			throw std::runtime_error("Memory::Memory(): Resource is null");

		if (resource->IsBound())
			throw std::runtime_error("Memory::Memory(): Resource is already bound");

		IResource::Requirements requirements = resource->GetRequirements();

		_Size += requirements.Size;
		memory_type_bits &= requirements.MemoryTypes;
	}

	_MemoryType.Set(logical_device->GetPhysicalDevice()->FindMemoryType(memory_type_bits, allocation_type, target_properties), this);

	VkMemoryAllocateInfo vk_memory_allocation_info{};
	vk_memory_allocation_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vk_memory_allocation_info.allocationSize = _Size;
	vk_memory_allocation_info.memoryTypeIndex = _MemoryType->GetTypeID();

	VkResult vk_result = vkAllocateMemory(_LogicalDevice->GetVKLogicalDevice(), &vk_memory_allocation_info, nullptr, &_VKDeviceMemory);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Memory::Memory(): Failed to allocate memory");

	uint64 offset = 0;

	for (uint32 i = 0; i < resources.Length(); i++) {
		resources[i]->Bind(this, offset);
		offset += resources[i]->GetRequirements().Size;
	}
}

Memory::~Memory() {
	Destroy();
	NotifyDestruction(this);

	vkFreeMemory(_LogicalDevice->GetVKLogicalDevice(), _VKDeviceMemory, nullptr);
}

/* ---- ---- ---- ---- */

void* Memory::MapMemory(uint64 size, uint64 offset) {
	void* data;
	VkResult vk_result = vkMapMemory(_LogicalDevice->GetVKLogicalDevice(), _VKDeviceMemory, offset, size, 0, &data);
	if (vk_result != VK_SUCCESS)
		throw std::runtime_error("Memory::MapMemory(): Failed to map memory");

	return data;
}

void Memory::UnmapMemory() {
	vkUnmapMemory(_LogicalDevice->GetVKLogicalDevice(), _VKDeviceMemory);
}
