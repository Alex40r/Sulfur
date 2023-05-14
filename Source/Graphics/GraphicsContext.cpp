#include "GraphicsContext.hpp"

GraphicsContext::GraphicsContext() {
	NotifyCreation(this);
}

GraphicsContext::~GraphicsContext() {
	DestroyChildren();
	NotifyDestruction(this);
}