#pragma once

#include "Utils/Utils.hpp"

#include "IWindowContext.hpp"

class WindowContext : public IWindowContext, Parent<GraphicsContext> {
public:
	static Handle<WindowContext> Create(const Handle<GraphicsContext>& graphics_context) {
		return new WindowContext(graphics_context);
	}

private:
	WindowContext(const Handle<GraphicsContext>& graphics_context);

public:
	~WindowContext() override;

	Handle<WindowContext> GetWindowContext() override { return this; }

	/* ---- ---- ---- ---- */

	const List<Handle<Monitor>>& GetMonitors() { return Monitors; }
	const Handle<Monitor>& GetMonitor(uint32 monitor_id) { return Monitors[monitor_id]; }
	uint32 GetMonitorCount() { return Monitors.GetLength(); }

	/* ---- ---- ---- ---- */
private:
	static int GLFWInitialized;

	List<Handle<Monitor>> Monitors;
};