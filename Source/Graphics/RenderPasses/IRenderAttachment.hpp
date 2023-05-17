#pragma once

#include "Utils/Utils.hpp"

interface IRenderAttachment : public Object {
protected:
    IRenderAttachment() = default;

public:
    virtual ~IRenderAttachment() = default;

    virtual Handle<RenderAttachment> GetRenderAttachment() = 0;

    /* ---- ---- ---- ---- */
};