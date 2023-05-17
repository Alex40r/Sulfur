#include "Window.hpp"

#include "Monitor.hpp"

Window::Window(const Handle<WindowContext>& window_context,
			   uint32 width,
			   uint32 height,
			   std::string title,
			   const Handle<Monitor>& monitor)
	: Parent(window_context) {
	NotifyCreation(this);

	if (Parent<WindowContext>::Get().IsInvalid())
		throw std::runtime_error("Window must be created with a valid WindowContext");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	if (monitor.IsValid()) {
		monitor->UpdateVideoMode();

		glfwWindowHint(GLFW_RED_BITS, monitor->GetRedBits());
		glfwWindowHint(GLFW_GREEN_BITS, monitor->GetGreenBits());
		glfwWindowHint(GLFW_BLUE_BITS, monitor->GetBlueBits());
		glfwWindowHint(GLFW_REFRESH_RATE, monitor->GetRefreshRate());

		if (width == INVALID_ID)
			width = monitor->GetWidth();
		if (height == INVALID_ID)
			height = monitor->GetHeight();
	}

	GLFWWindow = glfwCreateWindow(width, height, title.c_str(), monitor.IsValid() ? monitor->GetGLFWMonitor() : nullptr, nullptr);
	if (GLFWWindow == nullptr)
		throw std::runtime_error("Failed to create window");

	glfwSetWindowUserPointer(GLFWWindow, this);
	glfwSetFramebufferSizeCallback(GLFWWindow, &Window::WindowResizeCallback);
}

Window::~Window() {
	DestroyChildren();
	NotifyDestruction(this);
}

void Window::SetWindowed(uint32 width, uint32 height, int32 x, int32 y) {
	glfwSetWindowMonitor(GLFWWindow, nullptr, x, y, width, height, GLFW_DONT_CARE);
}

void Window::SetFullscreen(const Handle<Monitor>& monitor, uint32 width, uint32 height) {
	if (monitor.IsInvalid())
		throw std::runtime_error("Monitor must be valid to enter fullscreen");

	monitor->UpdateVideoMode();

	if (width == INVALID_ID)
		width = monitor->GetWidth();
	if (height == INVALID_ID)
		height = monitor->GetHeight();

	glfwSetWindowMonitor(GLFWWindow, monitor->GetGLFWMonitor(), 0, 0, width, height, monitor->GetRefreshRate());
}

void Window::SetTitle(std::string title) {
	glfwSetWindowTitle(GLFWWindow, title.c_str());
}

void Window::Resize(int32 width, int32 height) {
	glfwSetWindowSize(GLFWWindow, width, height);
}

void Window::SetWindowPosition(int32 x, int32 y) {
	glfwSetWindowPos(GLFWWindow, x, y);
}

void Window::Show() {
	glfwShowWindow(GLFWWindow);
}

void Window::Hide() {
	glfwHideWindow(GLFWWindow);
}

void Window::PollEvents() {
	glfwPollEvents();
}

bool Window::IsVisible() {
	int32 width, height;
	glfwGetFramebufferSize(GLFWWindow, &width, &height);
	return width != 0 && height != 0;
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(GLFWWindow);
}

void Window::WindowResizeCallback(GLFWwindow* glfw_window, int32 width, int32 height) {
}

/* ---- ---- ---- ---- */