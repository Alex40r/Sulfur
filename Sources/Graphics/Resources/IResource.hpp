#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

interface IResource : public Object {
public:
	struct Requirements {
		Requirements() = default;
		Requirements(uint64 size, uint32 memory_types, uint64 alignment);
		Requirements(const VkMemoryRequirements& vk_requirements);

		uint64 Size;
		uint32 MemoryTypes;
		uint64 Alignment;
	};

protected:
	IResource() = default;

public:
	virtual ~IResource() = default;

	/* ---- ---- ---- ---- */

	virtual bool IsBound() const = 0;
	virtual void Bind(const Handle<Memory>& memory, uint64 offset = 0) = 0;
	virtual Requirements GetRequirements() const = 0;
};