#pragma once

#include "Definitions.hpp"

#include "THandle.hpp"
#include "TObject.hpp"

template <class T>
class Handle : public THandle {
	template <class>
	friend class Handle;
	friend class TObject;

public:
	static const Handle<T>& Null;

	Handle() = default;
	Handle(T* pointer);
	Handle(std::nullptr_t);
	Handle(const Handle<T>& other);
	template <class S>
	Handle(const Handle<S>& other);
	Handle(Handle<T>&& other);
	~Handle() override;

	Handle<T>& operator=(T* pointer);
	Handle<T>& operator=(std::nullptr_t);
	Handle<T>& operator=(const Handle<T>& other);
	template <class S>
	Handle<T>& operator=(const Handle<S>& other);
	Handle<T>& operator=(Handle<T>&& other);

	bool operator==(T* pointer) const;
	bool operator==(std::nullptr_t) const;
	bool operator==(const Handle<T>& other) const;
	template <class S>
	bool operator==(const Handle<S>& other) const;

	bool operator!=(T* pointer) const;
	bool operator!=(std::nullptr_t) const;
	bool operator!=(const Handle<T>& other) const;
	template <class S>
	bool operator!=(const Handle<S>& other) const;

	T* operator->() const;
	T& operator*() const;
	operator T*() const;

	T* GetPointer() const;

	bool IsInvalid() const;
	bool IsValid() const;
	bool Equals(T* pointer) const;
	bool Equals(const Handle<T>& other) const;
	template <class S>
	bool Equals(const Handle<S>& other) const;

	void Reset();
	void Reset(const Handle<T>& other);
	void Reset(T* pointer);

	void DestroyObject() const;

	/* ---- ---- ---- ---- */
private:
	void Reset(T* pointer, TObject* object);
	void Move(Handle<T>&& other);

	void Destroy() override;
	void Trigger() override;

	/* ---- ---- ---- ---- */

	T* _Pointer = nullptr;
};

/* ---- ---- ---- ---- */

template <class T>
const Handle<T>& Handle<T>::Null = nullptr;

template <class T>
inline Handle<T>::Handle(T* pointer) {
	Reset(pointer);
}

template <class T>
inline Handle<T>::Handle(std::nullptr_t) {
}

template <class T>
inline Handle<T>::Handle(const Handle<T>& other) {
	Reset(other);
}

template <class T>
template <class S>
inline Handle<T>::Handle(const Handle<S>& other) {
	Reset(static_cast<T*>(other._Pointer), other._Object);
}

template <class T>
inline Handle<T>::Handle(Handle<T>&& other) {
	Move(std::move(other));
}

template <class T>
inline Handle<T>::~Handle() {
	Reset();
}

template <class T>
inline Handle<T>& Handle<T>::operator=(T* pointer) {
	Reset(pointer);

	return *this;
}

template <class T>
inline Handle<T>& Handle<T>::operator=(std::nullptr_t) {
	Reset();

	return *this;
}

template <class T>
inline Handle<T>& Handle<T>::operator=(const Handle<T>& other) {
	Reset(other);

	return *this;
}

template <class T>
template <class S>
inline Handle<T>& Handle<T>::operator=(const Handle<S>& other) {
	Reset(static_cast<T*>(other._Pointer), other._Object);

	return *this;
}

template <class T>
inline Handle<T>& Handle<T>::operator=(Handle<T>&& other) {
	Move(std::move(other));

	return *this;
}

template <class T>
inline bool Handle<T>::operator==(T* pointer) const {
	return Equals(pointer);
}

template <class T>
inline bool Handle<T>::operator==(std::nullptr_t) const {
	return IsInvalid();
}

template <class T>
inline bool Handle<T>::operator==(const Handle<T>& other) const {
	return Equals(other);
}

template <class T>
template <class S>
inline bool Handle<T>::operator==(const Handle<S>& other) const {
	return Equals(other);
}

template <class T>
inline bool Handle<T>::operator!=(T* pointer) const {
	return !Equals(pointer);
}

template <class T>
inline bool Handle<T>::operator!=(std::nullptr_t) const {
	return !IsInvalid();
}

template <class T>
inline bool Handle<T>::operator!=(const Handle<T>& other) const {
	return !Equals(other);
}

template <class T>
template <class S>
inline bool Handle<T>::operator!=(const Handle<S>& other) const {
	return !Equals(other);
}

template <class T>
inline T* Handle<T>::operator->() const {
	return _Pointer;
}

template <class T>
inline T& Handle<T>::operator*() const {
	return *_Pointer;
}

template <class T>
inline Handle<T>::operator T*() const {
	return _Pointer;
}

template <class T>
inline T* Handle<T>::GetPointer() const {
	return _Pointer;
}

template <class T>
inline bool Handle<T>::IsInvalid() const {
	return _Object == nullptr;
}

template <class T>
inline bool Handle<T>::IsValid() const {
	return !IsInvalid();
}

template <class T>
inline bool Handle<T>::Equals(T* pointer) const {
	return _Pointer == pointer;
}

template <class T>
inline bool Handle<T>::Equals(const Handle<T>& other) const {
	return _Pointer == other._Pointer;
}

template <class T>
template <class S>
inline bool Handle<T>::Equals(const Handle<S>& other) const {
	return _Object == other._Object;
}

template <class T>
inline void Handle<T>::Reset() {
	if (IsInvalid())
		return;

	if (_Tailward != nullptr)
		_Tailward->_Headward = _Headward;

	if (_Headward != nullptr)
		_Headward->_Tailward = _Tailward;
	else
		_Object->_HandlesHead = _Tailward;

	_Pointer = nullptr;
	_Object = nullptr;
}

template <class T>
inline void Handle<T>::Reset(const Handle<T>& other) {
	Reset(other._Pointer, other._Object);
}

template <class T>
inline void Handle<T>::Reset(T* pointer) {
	Reset(pointer, pointer);
}

template <class T>
inline void Handle<T>::DestroyObject() const {
	TObject::Destroy(*this);
}

template <class T>
inline void Handle<T>::Reset(T* pointer, TObject* object) {
	Reset();

	if (pointer == nullptr || object == nullptr)
		return;

	_Pointer = pointer;
	_Object = object;

	_Headward = nullptr;
	_Tailward = _Object->_HandlesHead;
	_Object->_HandlesHead = this;

	if (_Tailward != nullptr)
		_Tailward->_Headward = this;
}

template <class T>
inline void Handle<T>::Move(Handle<T>&& other) {
	Reset(other._Pointer, other._Object);
	
	other.Reset();
}

template <class T>
inline void Handle<T>::Destroy() {
	Reset();
}

template <class T>
inline void Handle<T>::Trigger() {
}
