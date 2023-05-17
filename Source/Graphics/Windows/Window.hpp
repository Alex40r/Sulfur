#pragma once

#include "Utils/Utils.hpp"

#include "External/GLFW.hpp"

#include "IWindow.hpp"

class Window : public IWindow, Parent<WindowContext> {
public:
	static Handle<Window> Create(const Handle<WindowContext>& window_context,
								 std::string title,
								 uint32 width,
								 uint32 height) {
		return new Window(window_context, width, height, title, nullptr);
	}
	static Handle<Window> Create(const Handle<WindowContext>& window_context,
								 std::string title,
								 const Handle<Monitor>& monitor,
								 uint32 width = INVALID_ID,
								 uint32 height = INVALID_ID) {
		return new Window(window_context, width, height, title, monitor);
	}

private:
	Window(const Handle<WindowContext>& window_context,
		   uint32 width,
		   uint32 height,
		   std::string title,
		   const Handle<Monitor>& monitor);

public:
	~Window() override;

	Handle<Window> GetWindow() override { return this; }

	/* ---- ---- ---- ---- */

	void SetWindowed(uint32 width, uint32 height, int32 x, int32 y);
	void SetFullscreen(const Handle<Monitor>& monitor, uint32 width = INVALID_ID, uint32 height = INVALID_ID);
	void SetTitle(std::string title);
	void Resize(int32 width, int32 height);
	void SetWindowPosition(int32 x, int32 y);

	void Show();
	void Hide();

	bool IsVisible();
	bool ShouldClose();

	static void PollEvents();

private:
	static void WindowResizeCallback(GLFWwindow* glfw_window, int32 width, int32 height);

	/* ---- ---- ---- ---- */

	GLFWwindow* GLFWWindow = nullptr;
};