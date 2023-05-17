#pragma once

#include "Utils/Utils.hpp"

interface IWindowContext : public Object {
protected:
	IWindowContext() = default;

public:
	virtual ~IWindowContext() = default;

	virtual Handle<WindowContext> GetWindowContext() = 0;

	/* ---- ---- ---- ---- */
};