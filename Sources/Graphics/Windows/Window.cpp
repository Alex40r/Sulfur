#include "Window.hpp"

#include "Monitor.hpp"

Window::Window(const Handle<WindowContext>& window_context,
			   const std::string& title,
			   uint32 width,
			   uint32 height,
			   const Handle<Monitor>& monitor)
	: _WindowContext(window_context, this) {
	NotifyCreation(this);

	if (_WindowContext.IsInvalid())
		throw std::runtime_error("Window::Window(): GraphicsContext is null");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	if (monitor.IsValid()) {
		monitor->UpdateVideoInfo();

		glfwWindowHint(GLFW_RED_BITS, monitor->GetRedBits());
		glfwWindowHint(GLFW_GREEN_BITS, monitor->GetGreenBits());
		glfwWindowHint(GLFW_BLUE_BITS, monitor->GetBlueBits());
		glfwWindowHint(GLFW_REFRESH_RATE, monitor->GetRefreshRate());

		if (width == INVALID_ID)
			width = monitor->GetWidth();
		if (height == INVALID_ID)
			height = monitor->GetHeight();
	}

	GLFWWindow = glfwCreateWindow(width, height, title.c_str(), monitor.IsInvalid() ? nullptr : monitor->GetGLFWMonitor(), nullptr);
	if (GLFWWindow == nullptr)
		throw std::runtime_error("Window::Window(): Failed to create GLFW window");

	glfwSetWindowUserPointer(GLFWWindow, this);
}

Window::~Window() {
	Destroy();
	NotifyDestruction(this);

	glfwDestroyWindow(GLFWWindow);
}

/* ---- ---- ---- ---- */

void Window::SetWindowed(uint32 width, uint32 height, int32 x, int32 y) {
	glfwSetWindowMonitor(GLFWWindow, nullptr, x, y, width, height, GLFW_DONT_CARE);
}

void Window::SetFullscreen(const Handle<Monitor>& monitor, uint32 width, uint32 height) {
	if (monitor.IsInvalid())
		throw std::runtime_error("Window::SetFullscreen(): Monitor is null");

	monitor->UpdateVideoInfo();

	if (width == INVALID_ID)
		width = monitor->GetWidth();
	if (height == INVALID_ID)
		height = monitor->GetHeight();

	glfwSetWindowMonitor(GLFWWindow, monitor->GetGLFWMonitor(), 0, 0, width, height, monitor->GetRefreshRate());
}

void Window::Resize(uint32 width, uint32 height) {
	glfwSetWindowSize(GLFWWindow, width, height);
}

void Window::SetWindowPosition(int32 x, int32 y) {
	glfwSetWindowPos(GLFWWindow, x, y);
}

void Window::SetTitle(const std::string& title) {
	glfwSetWindowTitle(GLFWWindow, title.c_str());
}

bool Window::IsFramebufferVisible() {
	int32 width, height;
	glfwGetFramebufferSize(GLFWWindow, &width, &height);
	return width != 0 && height != 0;
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(GLFWWindow);
}

void Window::PollEvents() {
	glfwPollEvents();
}

List<std::string> Window::GetRequiredInstanceExtensions() {
	uint32 glfw_extension_count;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	List<std::string> extensions(glfw_extension_count);
	for (uint32 i = 0; i < glfw_extension_count; i++)
		extensions[i] = glfw_extensions[i];

	return extensions;
}