#pragma once

#include "Utils/Utils.hpp"

interface IWindow : public Object {
protected:
    IWindow() = default;

public:
    virtual ~IWindow() = default;

    virtual Handle<Window> GetWindow() = 0;

    /* ---- ---- ---- ---- */
};