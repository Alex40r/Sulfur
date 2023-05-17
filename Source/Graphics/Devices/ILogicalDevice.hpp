#pragma once

#include "Utils/Utils.hpp"

interface ILogicalDevice : public Object {
protected:
    ILogicalDevice() = default;

public:
    virtual ~ILogicalDevice() = default;

    virtual Handle<LogicalDevice> GetLogicalDevice() = 0;

    /* ---- ---- ---- ---- */
};