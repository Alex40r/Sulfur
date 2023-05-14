#pragma once

#include "External/GLFW.hpp"
#include "Utils/Utils.hpp"

class Monitor : public Object<WindowContext> {
	friend class WindowContext;

private:
	static Handle<Monitor> Create(const Handle<WindowContext>& window_context,
								  GLFWmonitor* glfw_monitor) {
		return new Monitor(window_context, glfw_monitor);
	}

	Monitor(const Handle<WindowContext>& window_context,
			GLFWmonitor* glfw_monitor);

public:
	~Monitor();

	bool IsPrimary() const { return glfwGetPrimaryMonitor() == GLFWMonitor; }
	std::string GetName() const { return glfwGetMonitorName(GLFWMonitor); }

	uint32 GetWidth() const { return GLFWVideoMode->width; }
	uint32 GetHeight() const { return GLFWVideoMode->height; }
	uint32 GetRefreshRate() const { return GLFWVideoMode->refreshRate; }

	uint32 GetRedBits() const { return GLFWVideoMode->redBits; }
	uint32 GetGreenBits() const { return GLFWVideoMode->greenBits; }
	uint32 GetBlueBits() const { return GLFWVideoMode->blueBits; }

	GLFWmonitor* GetGLFWMonitor() const { return GLFWMonitor; }

private:
	GLFWmonitor* GLFWMonitor;
	const GLFWvidmode* GLFWVideoMode;
};