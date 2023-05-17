#pragma once

#include "Utils/Utils.hpp"

interface IPhysicalDevice : public Object {
protected:
    IPhysicalDevice() = default;

public:
    virtual ~IPhysicalDevice() = default;

    virtual Handle<PhysicalDevice> GetPhysicalDevice() = 0;

    /* ---- ---- ---- ---- */
};