#pragma once

#include "Definitions.hpp"

class Utils::THandle : public Unmovable {
	friend class Utils;

protected:
	THandle() = default;
	~THandle();

	virtual void Destroy() {}

private:
	Utils::TObject* GetObject() const { return Object; }

	Utils::THandle* NextHandle = nullptr;
	Utils::THandle* PreviousHandle = nullptr;

	Utils::TObject* Object = nullptr;
};