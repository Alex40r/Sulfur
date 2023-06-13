#pragma once

#include "Utils/Utils.hpp"

class GraphicsContext : public Object {
public:
    static Handle<GraphicsContext> Create() {
        return new GraphicsContext();
    }

private:
    GraphicsContext();

public:
    ~GraphicsContext() override;

    /* ---- ---- ---- ---- */

    /* ---- ---- ---- ---- */

private:
    /* ---- ---- ---- ---- */
};