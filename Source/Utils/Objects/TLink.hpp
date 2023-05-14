#pragma once

#include "Definitions.hpp"

class Utils::TLink : public Fixed {
	friend class Utils;

protected:
	TLink() = default;
	~TLink() = default;

	Utils::TLink* NextLink = nullptr;
	Utils::TLink* PreviousLink = nullptr;

	Utils::TObject* ParentObject = nullptr;
	Utils::TObject* ChildObject = nullptr;
};