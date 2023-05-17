#pragma once

#include "Utils/Utils.hpp"

interface ICommandPool : public Object {
protected:
    ICommandPool() = default;

public:
    virtual ~ICommandPool() = default;

    virtual Handle<CommandPool> GetCommandBufferPool() = 0;

    /* ---- ---- ---- ---- */
};