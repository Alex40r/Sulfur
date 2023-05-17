#pragma once

#include "Definitions.hpp"

#include "Utils/Generics/List.hpp"

#include "Link.hpp"
#include "TObject.hpp"

template <class P>
class Parent : public virtual TObject {
public:
	using Type = Handle<P>;

protected:
	Parent() = default;
	Parent(const Handle<P>& parent) { SetParent(parent); }

	void SetParent(const Handle<P>& parent);
	void UnsetParent();

	const Handle<P>& Get() { return ParentHandle; }

private:
	Handle<P> ParentHandle;
	Link<P> ParentLink;
};

/* ---- ---- ---- ---- */

template <class P>
inline void Parent<P>::SetParent(const Handle<P>& parent) {
	ParentHandle = parent;
	ParentLink.Init(parent.GetObject(), static_cast<TObject*>(this));
}

template <class P>
inline void Parent<P>::UnsetParent() {
	ParentLink.Destroy();
	ParentHandle = nullptr;
}

/* ---- ---- ---- ---- */

template <class P>
class Parent<List<P>> : public virtual TObject {
public:
	using Type = List<Handle<P>>;

protected:
	Parent() = default;
	Parent(const List<Handle<P>>& parents) { SetParent(parents); }

	void SetParent(const List<Handle<P>>& parents);
	void UnsetParent();

	const List<Handle<P>>& Get() { return ParentHandles; }

private:
	List<Handle<P>> ParentHandles;
	List<Link<P>> ParentLinks;
};

/* ---- ---- ---- ---- */

template <class P>
inline void Parent<List<P>>::SetParent(const List<Handle<P>>& parents) {
	ParentHandles = parents;
	ParentLinks.Clear();
	ParentLinks.Resize(parents.GetLength());

	for (uint32 i = 0; i < parents.GetLength(); i++)
		ParentLinks[i].Init(parents[i].GetObject(), static_cast<TObject*>(this));
}

template <class P>
inline void Parent<List<P>>::UnsetParent() {
	ParentLinks.Clear();
	ParentHandles.Clear();
}
