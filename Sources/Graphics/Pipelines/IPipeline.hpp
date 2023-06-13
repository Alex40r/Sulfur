#pragma once

#include "Utils/Utils.hpp"

#include "External/Vulkan.hpp"

interface IPipeline : public Object {
protected:
	IPipeline() = default;

public:
	virtual ~IPipeline() = default;

	/* ---- ---- ---- ---- */

    virtual VkPipeline GetVKPipeline() const = 0;
};