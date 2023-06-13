#include "GraphicsContext.hpp"

GraphicsContext::GraphicsContext() {
    NotifyCreation(this);
}

GraphicsContext::~GraphicsContext() {
    Destroy();
    NotifyDestruction(this);
}

/* ---- ---- ---- ---- */