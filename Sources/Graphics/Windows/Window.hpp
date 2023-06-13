#pragma once

#include "Utils/Utils.hpp"

#include "External/GLFW.hpp"

class Window : public Object {
public:
	static Handle<Window> Create(const Handle<WindowContext>& window_context,
								 const std::string& title,
								 uint32 width,
								 uint32 height) {
		return new Window(window_context, title, width, height, nullptr);
	}
	static Handle<Window> Create(const Handle<WindowContext>& window_context,
								 const Handle<Monitor>& monitor,
								 const std::string& title,
								 uint32 width = INVALID_ID,
								 uint32 height = INVALID_ID) {
		return new Window(window_context, title, width, height, monitor);
	}

private:
	Window(const Handle<WindowContext>& window_context,
		   const std::string& title,
		   uint32 width,
		   uint32 height,
		   const Handle<Monitor>& monitor);

public:
	~Window() override;

	/* ---- ---- ---- ---- */

	GLFWwindow* GetGLFWWindow() const { return GLFWWindow; }

	void SetWindowed(uint32 width, uint32 height, int32 x, int32 y);
	void SetFullscreen(const Handle<Monitor>& monitor, uint32 width = INVALID_ID, uint32 height = INVALID_ID);

	void Resize(uint32 width, uint32 height);
	void SetWindowPosition(int32 x, int32 y);

	void SetTitle(const std::string& title);

	bool IsFramebufferVisible();
	bool ShouldClose();

	static void PollEvents();

	List<std::string> GetRequiredInstanceExtensions();

	/* ---- ---- ---- ---- */

	const ParentHandle<WindowContext>& GetWindowContext() const { return _WindowContext; }

private:
	ParentHandle<WindowContext> _WindowContext;

	/* ---- ---- ---- ---- */

	GLFWwindow* GLFWWindow = nullptr;
};