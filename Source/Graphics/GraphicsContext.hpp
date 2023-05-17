#pragma once

#include "Utils/Utils.hpp"

#include "IGraphicsContext.hpp"

class GraphicsContext : public IGraphicsContext  {
public:
    static Handle<GraphicsContext> Create() {
        return new GraphicsContext();
    }

private:
    GraphicsContext();

public:
    ~GraphicsContext() override;

    Handle<GraphicsContext> GetGraphicsContext() override { return this; }

    /* ---- ---- ---- ---- */

    /* ---- ---- ---- ---- */
private:
};