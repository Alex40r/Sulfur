#pragma once

#include "Definitions.hpp"

#include "TLink.hpp"
#include "TObject.hpp"

template <class P>
class Utils::Link : public Utils::TLink {
public:
	Link() = default;
	Link(const Link<P>& link);
	~Link();

	void InitLink(const Handle<P>& parent, Utils::TObject* child);
	void DestroyLink();

	bool IsValid() { return Parent.IsValid(); };
	bool IsInvalid() { return Parent.IsInvalid(); };

	Handle<P> Parent;
};

/* ---- ---- ---- ---- */

template <class P>
inline Utils::Link<P>::Link(const Link<P>& link) {
	InitLink(link.Parent, link.ChildObject);
}

template <class P>
inline Utils::Link<P>::~Link() {
	DestroyLink();
}

template <class P>
inline void Utils::Link<P>::InitLink(const Handle<P>& parent, Utils::TObject* child) {
	DestroyLink();

	if (parent.IsInvalid())
		return;

	Parent = parent;
	ParentObject = parent.GetObject();
	ChildObject = child;

	PreviousLink = ParentObject->Children;
	if (PreviousLink != nullptr)
		PreviousLink->NextLink = this;

	NextLink = nullptr;
	ParentObject->Children = this;
}

template <class P>
inline void Utils::Link<P>::DestroyLink() {
	if (ParentObject == nullptr)
		return;

	if (PreviousLink != nullptr)
		PreviousLink->NextLink = NextLink;

	if (NextLink != nullptr)
		NextLink->PreviousLink = PreviousLink;
	else
		ParentObject->Children = PreviousLink;

	ParentObject = nullptr;
	ChildObject = nullptr;
	Parent = nullptr;
}