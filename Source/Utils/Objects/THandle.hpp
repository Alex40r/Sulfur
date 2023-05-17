#pragma once

#include "Definitions.hpp"

#include "IHandle.hpp"

class THandle : public IHandle {
	template <class T>
	friend class Handle;

protected:
	THandle() = default;
	~THandle() = default;

	/* ---- ---- ---- ---- */
private:
	THandle* NextHandle = nullptr;
	THandle* PreviousHandle = nullptr;

	TObject* Object = nullptr;
};