#pragma once

#include "Utils/Utils.hpp"

interface ICommandQueueFamily : public Object {
protected:
    ICommandQueueFamily() = default;

public:
    virtual ~ICommandQueueFamily() = default;

    virtual Handle<CommandQueueFamily> GetCommandQueueFamily() = 0;

    /* ---- ---- ---- ---- */
};