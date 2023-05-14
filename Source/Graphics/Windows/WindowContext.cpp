#include "WindowContext.hpp"

#include "Monitor.hpp"

int WindowContext::GLFWInitialized = 0;

WindowContext::WindowContext(const Handle<GraphicsContext>& graphics_context)
	: Object(graphics_context) {
	if (graphics_context.IsInvalid())
		throw std::runtime_error("Invalid graphics context");

	NotifyCreation(this);

	if (GLFWInitialized == 0)
		glfwInit();

	GLFWInitialized++;

	int monitor_count = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

	Monitors.Resize(monitor_count);

	for (int i = 0; i < monitor_count; i++)
		Monitors[i] = Monitor::Create(this, monitors[i]);
}

WindowContext::~WindowContext() {
	DestroyChildren();
	NotifyDestruction(this);

	GLFWInitialized--;

	if (GLFWInitialized == 0)
		glfwTerminate();
}