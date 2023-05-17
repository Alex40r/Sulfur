#pragma once

#include "Utils/Utils.hpp"

interface IMonitor : public Object {
protected:
	IMonitor() = default;

public:
	virtual ~IMonitor() = default;

	virtual Handle<Monitor> GetMonitor() = 0;

	/* ---- ---- ---- ---- */
};