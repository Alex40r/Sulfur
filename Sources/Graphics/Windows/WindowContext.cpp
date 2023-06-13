#include "WindowContext.hpp"

#include "Monitor.hpp"

int WindowContext::_GLFWInitialized = 0;

WindowContext::WindowContext(const Handle<GraphicsContext>& graphics_context)
	: _GraphicsContext(graphics_context, this) {
	NotifyCreation(this);

	if (_GraphicsContext.IsInvalid())
		throw std::runtime_error("WindowContext::WindowContext(): Graphics context is null");

	if (_GLFWInitialized == 0)
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("WindowContext::WindowContext(): Failed to initialize GLFW");

	_GLFWInitialized++;

	int monitor_count;
	GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

	_Monitors.Resize(monitor_count);
	for (int i = 0; i < monitor_count; i++)
		_Monitors[i] = new Monitor(this, i, monitors[i]);
}

WindowContext::~WindowContext() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */