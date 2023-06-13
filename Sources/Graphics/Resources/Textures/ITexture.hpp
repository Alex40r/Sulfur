#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

#include "Graphics/Resources/IResource.hpp"

interface ITexture : public IResource {
protected:
	ITexture() = default;

public:
	virtual ~ITexture() = default;

	/* ---- ---- ---- ---- */

    virtual VkImage GetVKImage() const = 0;
};