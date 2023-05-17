#pragma once

#include "Utils/Utils.hpp"

interface IDescriptorPool : public Object {
protected:
    IDescriptorPool() = default;

public:
    virtual ~IDescriptorPool() = default;

    virtual Handle<DescriptorPool> GetDescriptorPool() = 0;

    /* ---- ---- ---- ---- */
};