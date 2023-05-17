#include "Monitor.hpp"

Monitor::Monitor(const Handle<WindowContext>& window_context, GLFWmonitor* monitor)
	: Parent<WindowContext>(window_context)
	, GLFWMonitor(monitor) {
	NotifyCreation(this);

	if (Parent<WindowContext>::Get().IsInvalid())
		throw std::runtime_error("Monitor must be created with a valid WindowContext");

    UpdateVideoMode();
}

Monitor::~Monitor() {
	DestroyChildren();
	NotifyDestruction(this);
}

void Monitor::UpdateVideoMode() {
    GLFWVideoMode = glfwGetVideoMode(GLFWMonitor);

    if (GLFWVideoMode == nullptr)
        throw std::runtime_error("Failed to get monitor video mode information");
}

/* ---- ---- ---- ---- */