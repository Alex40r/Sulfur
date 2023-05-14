#include "Monitor.hpp"

Monitor::Monitor(const Handle<WindowContext>& window_context, GLFWmonitor* glfw_monitor)
	: Object(window_context)
	, GLFWMonitor(glfw_monitor) {
	if (window_context.IsInvalid())
		throw std::runtime_error("Invalid window context");

	NotifyCreation(this);

	GLFWVideoMode = glfwGetVideoMode(GLFWMonitor);
}

Monitor::~Monitor() {
	DestroyChildren();
	NotifyDestruction(this);
}