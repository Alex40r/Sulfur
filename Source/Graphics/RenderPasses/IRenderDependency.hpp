#pragma once

#include "Utils/Utils.hpp"

interface IRenderDependency : public Object {
protected:
    IRenderDependency() = default;

public:
    virtual ~IRenderDependency() = default;

    virtual Handle<RenderDependency> GetRenderDependency() = 0;

    /* ---- ---- ---- ---- */
};