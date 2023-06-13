#pragma once

#include "Utils/Utils.hpp"

class WindowContext : public Object {
public:
	static Handle<WindowContext> Create(const Handle<GraphicsContext>& graphics_context) {
		return new WindowContext(graphics_context);
	}

private:
	WindowContext(const Handle<GraphicsContext>& graphics_context);

public:
	~WindowContext() override;

	/* ---- ---- ---- ---- */

	const List<Handle<Monitor>>& GetMonitors() const { return _Monitors; }
	const Handle<Monitor>& GetMonitor(uint32 index) const { return _Monitors[index]; }
	uint32 GetMonitorCount() const { return _Monitors.Length(); }

	/* ---- ---- ---- ---- */

	const ParentHandle<GraphicsContext>& GetGraphicsContext() const { return _GraphicsContext; }

private:
	ParentHandle<GraphicsContext> _GraphicsContext;

	/* ---- ---- ---- ---- */

	static int _GLFWInitialized;

	List<Handle<Monitor>> _Monitors;
};