#pragma once

#include "Utils/Utils.hpp"

interface IMemoryType : public Object {
protected:
    IMemoryType() = default;

public:
    virtual ~IMemoryType() = default;

    virtual Handle<MemoryType> GetMemoryType() = 0;

    /* ---- ---- ---- ---- */
};