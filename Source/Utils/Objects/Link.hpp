#pragma once

#include "Definitions.hpp"

#include "TLink.hpp"

template <class P>
class Link : public TLink {
public:
	Link() = default;
	Link(const Link<P>& link);
	~Link();

	void Init(TObject* parent, TObject* child);
	void Destroy();
};

/* ---- ---- ---- ---- */

template <class P>
inline Link<P>::Link(const Link<P>& link) {
	Init(link.ParentObject, link.ChildObject);
}

template <class P>
inline Link<P>::~Link() {
	Destroy();
}

template <class P>
inline void Link<P>::Init(TObject* parent, TObject* child) {
	Destroy();

	if (parent == nullptr || child == nullptr)
		return;

	ParentObject = parent;
	ChildObject = child;

	NextLink = nullptr;
	PreviousLink = ParentObject->Children;

	if (PreviousLink != nullptr)
		PreviousLink->NextLink = this;

	ParentObject->Children = this;
}

template <class P>
inline void Link<P>::Destroy() {
	if (ParentObject == nullptr || ChildObject == nullptr)
		return;

	if (PreviousLink != nullptr)
		PreviousLink->NextLink = NextLink;

	if (NextLink != nullptr)
		NextLink->PreviousLink = PreviousLink;
	else
		ParentObject->Children = PreviousLink;

	ParentObject = nullptr;
	ChildObject = nullptr;
}
