#pragma once

#include "Definitions.hpp"

#include "Handle.hpp"
#include "Parent.hpp"
#include "ParentType.hpp"
#include "TObject.hpp"

#include <iostream>

template <class... P>
class Utils::Object : public virtual Utils::TObject, public Utils::Parent<P>... {
protected:
	Object(const typename Utils::ParentType<P>::Type&... parent);
	virtual ~Object();
};

/* ---- ---- ---- ---- */

template <class... P>
inline Utils::Object<P...>::Object(const typename Utils::ParentType<P>::Type&... parent)
	: Utils::Parent<P>(parent)... {
}
template <class... P>
inline Utils::Object<P...>::~Object(){
};

template <class P, class O>
inline const typename Utils::ParentType<P>::Type Utils::GetParent(O* parent) {
	static_assert(is_complete<O>::value, "Object class is incomplete");
	static_assert(is_complete<P>::value, "Parent class is incomplete");

	return static_cast<Utils::Parent<P>*>(parent)->GetParent();
}

template <class P, class O>
inline const typename Utils::ParentType<P>::Type Utils::GetParent(const Utils::Handle<O>& parent) {
	static_assert(is_complete<O>::value, "Object class is incomplete");
	static_assert(is_complete<P>::value, "Parent class is incomplete");

	return static_cast<Utils::Parent<P>*>(parent.GetPointer())->GetParent();
}
