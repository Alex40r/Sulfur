#pragma once

#include "Utils/Utils.hpp"

interface IInstance : public Object {
protected:
    IInstance() = default;

public:
    virtual ~IInstance() = default;

    virtual Handle<Instance> GetInstance() = 0;

    /* ---- ---- ---- ---- */
};