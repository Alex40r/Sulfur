#pragma once

#include "Utils/Utils.hpp"

interface ICommandBuffer : public Object {
protected:
    ICommandBuffer() = default;

public:
    virtual ~ICommandBuffer() = default;

    virtual Handle<CommandBuffer> GetCommandBuffer() = 0;

    /* ---- ---- ---- ---- */
};