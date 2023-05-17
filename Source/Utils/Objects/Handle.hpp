#pragma once

#include "Definitions.hpp"

#include "THandle.hpp"
#include "TObject.hpp"

template <class T>
class Handle : public THandle {
	template <class P>
	friend class Parent;

public:
	Handle() = default;
	Handle(T* pointer);
	Handle(std::nullptr_t);
	Handle(const Handle<T>& handle);
	template <class S>
	Handle(const Handle<S>& handle);

	~Handle();

	Handle<T>& operator=(T* pointer);
	Handle<T>& operator=(std::nullptr_t);
	Handle<T>& operator=(const Handle<T>& handle);
	template <class S>
	Handle<T>& operator=(const Handle<S>& handle);

	T* operator->() const;
	T* operator*() const;
	operator T*() const;

	bool operator==(const Handle<T>& handle) const;
	bool operator!=(const Handle<T>& handle) const;

	bool IsValid() const;
	bool IsInvalid() const;
	T* GetPointer() const;
	void DestroyObject();

	void DestroyHandle();
	void InitHandle(T* target);

private:
	void InitHandle(T* target, TObject* object);

	void Destroy() override;
	bool Matches(const TObject* object) const override;

	TObject* GetObject() const;

	/* ---- ---- ---- ---- */

	T* Pointer = nullptr;
};

/* ---- ---- ---- ---- */

template <class T>
inline Handle<T>::Handle(T* pointer) {
	InitHandle(pointer);
}

template <class T>
inline Handle<T>::Handle(std::nullptr_t) {
}

template <class T>
inline Handle<T>::Handle(const Handle<T>& handle) {
	InitHandle(handle.Pointer, handle.Object);
}

template <class T>
template <class S>
inline Handle<T>::Handle(const Handle<S>& handle) {
	InitHandle(static_cast<T*>(handle.Pointer), handle.Object);
}

template <class T>
inline Handle<T>::~Handle() {
	DestroyHandle();
}

template <class T>
inline Handle<T>& Handle<T>::operator=(T* pointer) {
	InitHandle(pointer);
	return *this;
}

template <class T>
inline Handle<T>& Handle<T>::operator=(std::nullptr_t) {
	DestroyHandle();
	return *this;
}

template <class T>
inline Handle<T>& Handle<T>::operator=(const Handle<T>& handle) {
	InitHandle(handle.Pointer, handle.Object);
	return *this;
}

template <class T>
template <class S>
inline Handle<T>& Handle<T>::operator=(const Handle<S>& handle) {
	InitHandle(static_cast<T*>(handle.Pointer), handle.Object);
	return *this;
}

template <class T>
inline T* Handle<T>::operator->() const {
	return Pointer;
}

template <class T>
inline T* Handle<T>::operator*() const {
	return Pointer;
}

template <class T>
inline Handle<T>::operator T*() const {
	return Pointer;
}

template <class T>
inline bool Handle<T>::operator==(const Handle<T>& handle) const {
	return Object == handle.Object;
}

template <class T>
inline bool Handle<T>::operator!=(const Handle<T>& handle) const {
	return Object != handle.Object;
}

template <class T>
inline bool Handle<T>::IsValid() const {
	return Object != nullptr;
}

template <class T>
inline bool Handle<T>::IsInvalid() const {
	return Object == nullptr;
}

template <class T>
inline T* Handle<T>::GetPointer() const {
	return Pointer;
}

template <class T>
inline void Handle<T>::DestroyObject() {
	if (IsInvalid())
		return;

	TObject::Destroy(Object);
}

template <class T>
inline void Handle<T>::DestroyHandle() {
	if (IsInvalid())
		return;

	if (PreviousHandle != nullptr)
		PreviousHandle->NextHandle = NextHandle;

	if (NextHandle != nullptr)
		NextHandle->PreviousHandle = PreviousHandle;
	else
		Object->Handles = PreviousHandle;

	Pointer = nullptr;
	Object = nullptr;
}

template <class T>
inline void Handle<T>::InitHandle(T* target) {
	InitHandle(target, static_cast<TObject*>(target));
}

template <class T>
inline void Handle<T>::InitHandle(T* target, TObject* object) {
	DestroyHandle();

	if (target == nullptr)
		return;

	Pointer = target;
	Object = object;

	NextHandle = nullptr;
	PreviousHandle = Object->Handles;

	if (PreviousHandle != nullptr)
		PreviousHandle->NextHandle = this;

	Object->Handles = this;
}

template <class T>
inline void Handle<T>::Destroy() {
	DestroyHandle();
}

template <class T>
inline bool Handle<T>::Matches(const TObject* object) const {
	return Object == object;
}

template <class T>
inline TObject* Handle<T>::GetObject() const {
	return Object;
}
