#pragma once

#include "Definitions.hpp"

class TLink : public Fixed {
	template <class P>
	friend class Link;

protected:
	TLink() = default;
	~TLink() = default;

	TLink* NextLink = nullptr;
	TLink* PreviousLink = nullptr;

	TObject* ParentObject = nullptr;
	TObject* ChildObject = nullptr;

public:
	bool Matches(TObject* parent) { return ParentObject == parent; }
	TObject* GetChild() { return ChildObject; }
};