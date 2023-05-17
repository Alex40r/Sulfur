#include "WindowContext.hpp"

#include "External/GLFW.hpp"

#include "Monitor.hpp"

int WindowContext::GLFWInitialized = 0;

WindowContext::WindowContext(const Handle<GraphicsContext>& graphics_context)
	: Parent<GraphicsContext>(graphics_context) {
	NotifyCreation(this);

	if (Parent<GraphicsContext>::Get().IsInvalid())
		throw std::runtime_error("WindowContext must be created with a valid GraphicsContext");

	if (GLFWInitialized == 0)
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("Failed to initialize GLFW");

	GLFWInitialized++;

	int monitor_count;
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

/* ---- ---- ---- ---- */