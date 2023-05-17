#pragma once

#include "Utils/Utils.hpp"

interface IMemoryHeap : public Object {
protected:
    IMemoryHeap() = default;

public:
    virtual ~IMemoryHeap() = default;

    virtual Handle<MemoryHeap> GetMemoryHeap() = 0;

    /* ---- ---- ---- ---- */
};