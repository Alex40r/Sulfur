#pragma once

#include "External/GLFW.hpp"
#include "Utils/Utils.hpp"

class WindowContext : public Object<GraphicsContext> {
public:
	static Handle<WindowContext> Create(const Handle<GraphicsContext>& graphics_context) {
		return new WindowContext(graphics_context);
	}

private:
	WindowContext(const Handle<GraphicsContext>& graphics_context);

public:
	~WindowContext();

	const List<Handle<Monitor>>& GetMonitors() { return Monitors; }

private:
	static int GLFWInitialized;

	List<Handle<Monitor>> Monitors;
};