#pragma once

#include "Utils/Utils.hpp"

interface IRenderPass : public Object {
protected:
    IRenderPass() = default;

public:
    virtual ~IRenderPass() = default;

    virtual Handle<RenderPass> GetRenderPass() = 0;

    /* ---- ---- ---- ---- */
};