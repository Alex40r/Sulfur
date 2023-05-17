#pragma once

#include "Utils/Utils.hpp"

interface IGraphicsContext : public Object {
protected:
	IGraphicsContext() = default;

public:
	virtual ~IGraphicsContext() = default;

	virtual Handle<GraphicsContext> GetGraphicsContext() = 0;

	/* ---- ---- ---- ---- */
};