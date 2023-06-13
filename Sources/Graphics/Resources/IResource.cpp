#include "IResource.hpp"

IResource::Requirements::Requirements(uint64 size, uint32 memory_types, uint64 alignment)
	: Size(size)
	, MemoryTypes(memory_types)
	, Alignment(alignment) {
}

IResource::Requirements::Requirements(const VkMemoryRequirements& vk_requirements)
	: Size(vk_requirements.size)
	, MemoryTypes(vk_requirements.memoryTypeBits)
	, Alignment(vk_requirements.alignment) {
}
