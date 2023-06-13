#pragma once

#include "Definitions.hpp"

#include "Handle.hpp"

template <class T>
class ParentHandle : public Handle<T> {
public:
	ParentHandle() = default;
	ParentHandle(const Handle<T>& target, TObject* source)
		: Handle<T>(target)
		, _Object(source) {
	}

	void SetTarget(const Handle<T>& target);
	void SetSource(TObject* source);

	void Set(const Handle<T>& target, TObject* source);

private:
	void Trigger() override;

	/* ---- ---- ---- ---- */
private:
	Handle<TObject> _Object;
};

/* ---- ---- ---- ---- */

template <class T>
inline void ParentHandle<T>::Trigger() {
	_Object.DestroyObject();
}

template <class T>
inline void ParentHandle<T>::SetTarget(const Handle<T>& target) {
	this->Reset(target);
}

template <class T>
inline void ParentHandle<T>::SetSource(TObject* source) {
	_Object = source;
}

template <class T>
inline void ParentHandle<T>::Set(const Handle<T>& target, TObject* source) {
	SetTarget(target);
	SetSource(source);
}
