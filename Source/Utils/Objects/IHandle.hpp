#pragma once

#include "Definitions.hpp"

class IHandle : public Unmovable {
	friend class TObject;

protected:
	IHandle() = default;
	~IHandle() = default;

	/* ---- ---- ---- ---- */

	virtual void Destroy() = 0;
	virtual bool Matches(const TObject* object) const = 0;
};