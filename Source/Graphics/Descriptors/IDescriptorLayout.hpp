#pragma once

#include "Utils/Utils.hpp"

interface IDescriptorLayout : public Object {
protected:
    IDescriptorLayout() = default;

public:
    virtual ~IDescriptorLayout() = default;

    virtual Handle<DescriptorLayout> GetDescriptorLayout() = 0;

    /* ---- ---- ---- ---- */
};