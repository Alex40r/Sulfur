#pragma once

#include "Definitions.hpp"

#include "THandle.hpp"
#include "TObject.hpp"

template <class T>
class Utils::Handle : public Utils::THandle {
	friend class Utils;

public:
	Handle() = default;
	Handle(T* pointer);
	Handle(std::nullptr_t);
	Handle(const Utils::Handle<T>& handle);
	template <class S>
	Handle(const Utils::Handle<S>& handle);
	~Handle();

	Utils::Handle<T>& operator=(T* pointer);
	Utils::Handle<T>& operator=(const Utils::Handle<T>& handle);
	template <class S>
	Utils::Handle<T>& operator=(const Utils::Handle<S>& handle);
	Utils::Handle<T>& operator=(std::nullptr_t);

	T* operator->() const;
	T* operator*() const;
	operator T*() const;

	bool operator==(const Utils::Handle<T>& handle) const;
	bool operator!=(const Utils::Handle<T>& handle) const;

	bool IsValid() const;
	bool IsInvalid() const;
	T* GetPointer() const;
	void DestroyObject();

	void InitHandle(T* target);

private:
	void InitHandle(T* target, Utils::TObject* object);

public:
	void DestroyHandle();

private:
	virtual void Destroy();

	T* Pointer = nullptr;
};

/* ---- ---- ---- ---- */

template <class T>
inline Utils::Handle<T>::Handle(std::nullptr_t) {
}

template <class T>
inline Utils::Handle<T>::Handle(T* pointer) {
	InitHandle(pointer);
}

template <class T>
inline Utils::Handle<T>::Handle(const Utils::Handle<T>& handle) {
	InitHandle(handle.Pointer, handle.Object);
}

template <class T>
template <class S>
inline Utils::Handle<T>::Handle(const Utils::Handle<S>& handle) {
	InitHandle(static_cast<T*>(handle.Pointer), handle.Object);
}

template <class T>
inline Utils::Handle<T>::~Handle() {
	DestroyHandle();
}

template <class T>
inline Utils::Handle<T>& Utils::Handle<T>::operator=(T* pointer) {
	InitHandle(pointer);
	return *this;
}

template <class T>
inline Utils::Handle<T>& Utils::Handle<T>::operator=(const Utils::Handle<T>& handle) {
	InitHandle(handle.Pointer, handle.Object);
	return *this;
}

template <class T>
template <class S>
inline Utils::Handle<T>& Utils::Handle<T>::operator=(const Utils::Handle<S>& handle) {
	InitHandle(handle.Pointer, handle.Object);
	return *this;
}

template <class T>
inline Utils::Handle<T>& Utils::Handle<T>::operator=(std::nullptr_t) {
	DestroyHandle();
	return *this;
}

template <class T>
inline T* Utils::Handle<T>::operator->() const {
	return Pointer;
}

template <class T>
inline T* Utils::Handle<T>::operator*() const {
	return Pointer;
}

template <class T>
inline Utils::Handle<T>::operator T*() const {
	return Pointer;
}

template <class T>
inline bool Utils::Handle<T>::operator==(const Utils::Handle<T>& handle) const {
	return Pointer == handle.Pointer;
}

template <class T>
inline bool Utils::Handle<T>::operator!=(const Utils::Handle<T>& handle) const {
	return Pointer != handle.Pointer;
}

template <class T>
inline bool Utils::Handle<T>::IsValid() const {
	return Pointer != nullptr;
}

template <class T>
inline bool Utils::Handle<T>::IsInvalid() const {
	return Pointer == nullptr;
}

template <class T>
inline T* Utils::Handle<T>::GetPointer() const {
	return Pointer;
}

template <class T>
inline void Utils::Handle<T>::DestroyObject() {
	if (IsInvalid())
		return;

	Utils::TObject::Destroy(Object);
}

template <class T>
inline void Utils::Handle<T>::InitHandle(T* target) {
	InitHandle(target, static_cast<Utils::TObject*>(target));
}

template <class T>
inline void Utils::Handle<T>::InitHandle(T* target, Utils::TObject* object) {
	DestroyHandle();

	if (target == nullptr)
		return;

	Object = object;
	Pointer = target;

	PreviousHandle = Object->Handles;
	if (PreviousHandle != nullptr)
		PreviousHandle->NextHandle = this;

	NextHandle = nullptr;
	Object->Handles = this;
}

template <class T>
inline void Utils::Handle<T>::DestroyHandle() {
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
inline void Utils::Handle<T>::Destroy() {
	DestroyHandle();
}
