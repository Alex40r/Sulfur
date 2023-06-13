#include "Monitor.hpp"

Monitor::Monitor(const Handle<WindowContext>& window_context,
				 uint32 index,
				 GLFWmonitor* monitor)
	: _WindowContext(window_context, this)
	, _GLFWMonitor(monitor) {
	NotifyCreation(this);

	if (_WindowContext.IsInvalid())
		throw std::runtime_error("Monitor::Monitor(): Graphics context is null");

	if (_GLFWMonitor == nullptr)
		throw std::runtime_error("Monitor::Monitor(): GLFW monitor is null");

	UpdateVideoInfo();
}

Monitor::~Monitor() {
	Destroy();
	NotifyDestruction(this);
}

/* ---- ---- ---- ---- */

void Monitor::UpdateVideoInfo() {
	_GLFWVideoInfo = glfwGetVideoMode(_GLFWMonitor);
	if (_GLFWVideoInfo == nullptr)
		throw std::runtime_error("Monitor::UpdateVideoInfo(): Failed to get GLFW video info");
}