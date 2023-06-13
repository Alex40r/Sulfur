#pragma once

#include "Definitions.hpp"

class THandle {
	template <class>
	friend class Handle;
	friend class TObject;

protected:
	THandle() = default;
	virtual ~THandle() = default;

	/* ---- ---- ---- ---- */
private:
	virtual void Destroy() = 0;
	virtual void Trigger() = 0;
	
	THandle* _Headward = nullptr;
	THandle* _Tailward = nullptr;

	TObject* _Object = nullptr;
};