#pragma once

#include "Utils/Utils.hpp"

interface IDescriptorSet : public Object {
protected:
    IDescriptorSet() = default;

public:
    virtual ~IDescriptorSet() = default;

    virtual Handle<DescriptorSet> GetDescriptorSet() = 0;

    /* ---- ---- ---- ---- */
};