#pragma once

#include "Utils/Utils.hpp"

interface ICommandQueue : public Object {
protected:
    ICommandQueue() = default;

public:
    virtual ~ICommandQueue() = default;

    virtual Handle<CommandQueue> GetCommandQueue() = 0;

    /* ---- ---- ---- ---- */
};