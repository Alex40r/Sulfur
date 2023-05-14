#include "Window.hpp"

#include "Monitor.hpp"

Window::Window(const Handle<WindowContext>& window_context, uint32 width, uint32 height, std::string title, const Handle<Monitor>& monitor)
	: Object(window_context) {
	if (window_context.IsInvalid())
		throw std::runtime_error("Invalid window context");

	NotifyCreation(this);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	if (monitor.IsValid()) {
		glfwWindowHint(GLFW_RED_BITS, monitor->GetRedBits());
		glfwWindowHint(GLFW_GREEN_BITS, monitor->GetGreenBits());
		glfwWindowHint(GLFW_BLUE_BITS, monitor->GetBlueBits());
		glfwWindowHint(GLFW_REFRESH_RATE, monitor->GetRefreshRate());
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

	glfwDestroyWindow(GLFWWindow);
}

void Window::SetWindowed(uint32 width, uint32 height) {
	glfwSetWindowMonitor(GLFWWindow, nullptr, 0, 0, width, height, GLFW_DONT_CARE);
}

void Window::SetFullscreen(uint32 width, uint32 height, const Handle<Monitor>& monitor) {
	glfwSetWindowMonitor(GLFWWindow, monitor->GetGLFWMonitor(), 0, 0, width, height, monitor->GetRefreshRate());
}

void Window::SetWindowPosition(int32 x, int32 y) {
	glfwSetWindowPos(GLFWWindow, x, y);
}

void Window::PollEvents() {
	glfwPollEvents();
}

List<std::string> Window::GetRequiredInstanceExtentions() {
	uint32 extention_count;
	const char** extentions;

	extentions = glfwGetRequiredInstanceExtensions(&extention_count);

	if (extentions == nullptr)
		throw std::runtime_error("Failed to get required instance extentions");

	List<std::string> list(extention_count);

	for (uint32 i = 0; i < extention_count; i++)
		list[i] = std::string(extentions[i]);

	return list;
}

void Window::WindowResizeCallback(GLFWwindow* glfw_window, int32 width, int32 height) {
	Handle<Window> window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
}
