#pragma once

#include "Utils/Utils.hpp"

#include "Graphics/Resources/IResource.hpp"

interface IBuffer : public IResource {
protected:
	IBuffer() = default;

public:
	virtual ~IBuffer() = default;

	/* ---- ---- ---- ---- */

	virtual VkBuffer GetVKBuffer() const = 0;
	virtual uint64 GetSize() const = 0;
};