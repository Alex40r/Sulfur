#pragma once

#include "External/GLFW.hpp"
#include "Utils/Utils.hpp"

class Window : public Object<WindowContext> {
public:
	static Handle<Window> Create(const Handle<WindowContext>& window_context,
								 uint32 width,
								 uint32 height,
								 std::string title,
								 const Handle<Monitor>& monitor = nullptr) {
		return new Window(window_context, width, height, title, monitor);
	}

private:
	Window(const Handle<WindowContext>& window_context,
		   uint32 width,
		   uint32 height,
		   std::string title,
		   const Handle<Monitor>& monitor);

public:
	~Window();

	void SetWindowed(uint32 width, uint32 height);
	void SetFullscreen(uint32 width, uint32 height, const Handle<Monitor>& monitor);
	void SetWindowPosition(int32 x, int32 y);

	void PollEvents();

	List<std::string> GetRequiredInstanceExtentions();

	GLFWwindow* GetGLFWWindow() { return GLFWWindow; };

private:
	static void WindowResizeCallback(GLFWwindow* glfw_window, int32 width, int32 height);

	GLFWwindow* GLFWWindow = nullptr;
};