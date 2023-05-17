#pragma once

#include "Utils/Utils.hpp"

#include "External/GLFW.hpp"

#include "IMonitor.hpp"

class Monitor : public IMonitor, Parent<WindowContext> {
	friend class WindowContext;

private:
	static Handle<Monitor> Create(const Handle<WindowContext>& window_context, GLFWmonitor* monitor) {
		return new Monitor(window_context, monitor);
	}

private:
	Monitor(const Handle<WindowContext>& window_context, GLFWmonitor* monitor);

public:
	~Monitor() override;

	Handle<Monitor> GetMonitor() override { return this; }

	/* ---- ---- ---- ---- */
	
	void UpdateVideoMode();

	uint32 GetRedBits() { return GLFWVideoMode->redBits; }
	uint32 GetGreenBits() { return GLFWVideoMode->greenBits; }
	uint32 GetBlueBits() { return GLFWVideoMode->blueBits; }

	uint32 GetRefreshRate() { return GLFWVideoMode->refreshRate; }

	uint32 GetWidth() { return GLFWVideoMode->width; }
	uint32 GetHeight() { return GLFWVideoMode->height; }

	GLFWmonitor* GetGLFWMonitor() { return GLFWMonitor; }

	/* ---- ---- ---- ---- */
private:
	GLFWmonitor* GLFWMonitor;
	const GLFWvidmode* GLFWVideoMode;
};