#pragma once

#include "Utils/Utils.hpp"

interface IRenderSubpass : public Object {
protected:
    IRenderSubpass() = default;

public:
    virtual ~IRenderSubpass() = default;

    virtual Handle<RenderSubpass> GetRenderSubpass() = 0;

    /* ---- ---- ---- ---- */
};