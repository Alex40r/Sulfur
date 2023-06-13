#pragma once

#include "Utils/Utils.hpp"

#include "External/GLFW.hpp"

class Monitor : public Object {
	friend class WindowContext;

private:
	Monitor(const Handle<WindowContext>& window_context,
			uint32 index,
			GLFWmonitor* monitor);

public:
	~Monitor() override;

	/* ---- ---- ---- ---- */

	GLFWmonitor* GetGLFWMonitor() const { return _GLFWMonitor; }

	void UpdateVideoInfo();

	uint32 GetRedBits() const { return _GLFWVideoInfo->redBits; }
	uint32 GetGreenBits() const { return _GLFWVideoInfo->greenBits; }
	uint32 GetBlueBits() const { return _GLFWVideoInfo->blueBits; }

	uint32 GetRefreshRate() const { return _GLFWVideoInfo->refreshRate; }

	uint32 GetWidth() const { return _GLFWVideoInfo->width; }
	uint32 GetHeight() const { return _GLFWVideoInfo->height; }

	/* ---- ---- ---- ---- */

	const ParentHandle<WindowContext>& GetWindowContext() const { return _WindowContext; }

private:
	ParentHandle<WindowContext> _WindowContext;

	/* ---- ---- ---- ---- */

	GLFWmonitor* _GLFWMonitor;

	const GLFWvidmode* _GLFWVideoInfo;
};