#include "PhysicalDevice.hpp"

#include "Graphics/CommandQueues/CommandQueueFamily.hpp"
#include "Graphics/Memory/MemoryHeap.hpp"
#include "Graphics/Memory/MemoryType.hpp"

PhysicalDevice::PhysicalDevice(const Handle<Instance>& instance,
							   uint32 index,
							   VkPhysicalDevice vk_physical_device)
	: _Instance(instance, this)
	, _PhysicalDeviceID(index)
	, _VKPhysicalDevice(vk_physical_device) {
	NotifyCreation(this);

	if (_Instance.IsInvalid())
		throw std::runtime_error("PhysicalDevice::PhysicalDevice(): Instance is null");

	vkGetPhysicalDeviceProperties(_VKPhysicalDevice, &_VKProperties);
	vkGetPhysicalDeviceFeatures(_VKPhysicalDevice, &_VKFeatures);
	vkGetPhysicalDeviceMemoryProperties(_VKPhysicalDevice, &_VKMemoryProperties);

	uint32 queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(_VKPhysicalDevice, &queue_family_count, nullptr);

	List<VkQueueFamilyProperties> vk_queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(_VKPhysicalDevice, &queue_family_count, vk_queue_families.Data());

	_CommandQueueFamilies.Resize(queue_family_count);
	for (uint32 i = 0; i < queue_family_count; i++)
		_CommandQueueFamilies[i] = new CommandQueueFamily(this, i, vk_queue_families[i]);

	_MemoryHeaps.Resize(_VKMemoryProperties.memoryHeapCount);
	for (uint32 i = 0; i < _VKMemoryProperties.memoryHeapCount; i++)
		_MemoryHeaps[i] = new MemoryHeap(this, i, _VKMemoryProperties.memoryHeaps[i]);

	_MemoryTypes.Resize(_VKMemoryProperties.memoryTypeCount);
	for (uint32 i = 0; i < _VKMemoryProperties.memoryTypeCount; i++)
		_MemoryTypes[i] = new MemoryType(_MemoryHeaps[_VKMemoryProperties.memoryTypes[i].heapIndex], i, _VKMemoryProperties.memoryTypes[i]);
}

PhysicalDevice::~PhysicalDevice() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

const Handle<MemoryType>& PhysicalDevice::FindMemoryType(uint32 memory_types, AllocationType allocation_type, VkMemoryPropertyFlags target_properties) const {
	switch (allocation_type) {
	case PREFER_LARGEST: {
		uint32 memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties);

		if (memory_type_id == INVALID_ID)
			throw std::runtime_error("PhysicalDevice::FindMemoryType(): Cannot find fitting memory type");

		return _MemoryTypes[memory_type_id];
	} break;
	case PREFER_DEVICE_LOCAL: {
		uint32 fallback_memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties);
		if (fallback_memory_type_id == INVALID_ID)
			throw std::runtime_error("PhysicalDevice::FindMemoryType(): Cannot find fitting memory type");

		uint32 memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (memory_type_id == INVALID_ID)
			memory_type_id = fallback_memory_type_id;

		return _MemoryTypes[memory_type_id];
	} break;
	case PREFER_HOST_VISIBLE: {
		uint32 fallback_memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties);
		if (fallback_memory_type_id == INVALID_ID)
			throw std::runtime_error("PhysicalDevice::FindMemoryType(): Cannot find fitting memory type");

		uint32 memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		if (memory_type_id == INVALID_ID)
			memory_type_id = fallback_memory_type_id;

		return _MemoryTypes[memory_type_id];
	} break;
	case PREFER_HOST_VISIBLE_COHERENT: {
		uint32 fallback_memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties);
		if (fallback_memory_type_id == INVALID_ID)
			throw std::runtime_error("PhysicalDevice::FindMemoryType(): Cannot find fitting memory type");

		uint32 memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (memory_type_id == INVALID_ID)
			memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		if (memory_type_id == INVALID_ID)
			memory_type_id = fallback_memory_type_id;

		return _MemoryTypes[memory_type_id];
	} break;
	case PREFER_HOST_VISIBLE_CACHED: {
		uint32 fallback_memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties);
		if (fallback_memory_type_id == INVALID_ID)
			throw std::runtime_error("PhysicalDevice::FindMemoryType(): Cannot find fitting memory type");

		uint32 memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);

		if (memory_type_id == INVALID_ID)
			memory_type_id = FindLargestMemoryTypeID(memory_types, target_properties | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		if (memory_type_id == INVALID_ID)
			memory_type_id = fallback_memory_type_id;

		return _MemoryTypes[memory_type_id];
	} break;
	default:
		throw std::runtime_error("PhysicalDevice::FindMemoryType(): Invalid allocation type");
	}
}

uint32 PhysicalDevice::FindLargestMemoryTypeID(uint32 memory_types, VkMemoryPropertyFlags target_properties) const {
	uint32 memory_type_id = INVALID_ID;
	uint64 heap_size = 0;

	for (uint32 i = 0; i < _MemoryTypes.Length(); i++) {
		if ((memory_types & (1 << i)) == 0)
			continue;

		if ((_MemoryTypes[i]->GetPropertyFlags() & target_properties) != target_properties)
			continue;

		if (memory_type_id == INVALID_ID ||
			_MemoryTypes[i]->GetMemoryHeap()->GetHeapSize() > heap_size) {
			memory_type_id = i;
			heap_size = _MemoryTypes[memory_type_id]->GetMemoryHeap()->GetHeapSize();
		}
	}

	return memory_type_id;
}