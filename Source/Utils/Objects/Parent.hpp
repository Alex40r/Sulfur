#pragma once

#include "Definitions.hpp"

#include "Link.hpp"

template <class P>
class Utils::Parent : public virtual TObject {
	friend class Utils;

public:
	using Type = Handle<P>;

protected:
	Parent(const Handle<P>& parent);
	void Assign(const Handle<P>& parent);
	void Unassign();
	const Handle<P>& GetParent();

private:
	Link<P> Link;
};

template <class P>
inline Utils::Parent<P>::Parent(const Handle<P>& parent) {
	Assign(parent);
}

template <class P>
inline void Utils::Parent<P>::Assign(const Handle<P>& parent) {
	Link.InitLink(parent, this);
}

template <class P>
inline void Utils::Parent<P>::Unassign() {
	Link.DestroyLink();
}

template <class P>
inline const Handle<P>& Utils::Parent<P>::GetParent() {
	return Link.Parent;
}

/* ---- ---- ---- ---- */

template <class P>
class Utils::Parent<List<P>> : public virtual TObject {
	friend class Utils;

public:
	using Type = List<Handle<P>>;

protected:
	Parent(const List<Handle<P>>& parent);
	void Assign(const List<Handle<P>>& parent);
	void Unassign();
	List<Handle<P>> GetParent();

private:
	List<Link<P>> Link;
};

template <class P>
inline Utils::Parent<List<P>>::Parent(const List<Handle<P>>& parent) {
	Assign(parent);
}

template <class P>
inline void Utils::Parent<List<P>>::Assign(const List<Handle<P>>& parent) {
	Link.Resize(parent.GetLength());
	for (uint32 i = 0; i < parent.GetLength(); i++)
		Link[i].InitLink(parent[i], this);
}

template <class P>
inline void Utils::Parent<List<P>>::Unassign() {
	for (uint32 i = 0; i < Link.GetLength(); i++)
		Link[i].DestroyLink();
	Link.Clear();
}

template <class P>
inline List<Handle<P>> Utils::Parent<List<P>>::GetParent() {
	List<Handle<P>> parent(Link.GetLength());
	for (uint32 i = 0; i < Link.GetLength(); i++)
		parent[i] = Link[i].Parent;
	return parent;
}
